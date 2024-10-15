#include <algorithm_ext.h>
#include <gamecontrollerclient.h>
#include <lola_comm.h>
#include <stl_ext.h>
#include <cinttypes>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <optionset.h>
#include <booloption.h>
#include <robotoption.h>

using namespace NaoControl;
static bool ignored = false;

GameControllerClient::GameControllerClient(uint8_t teamNr, uint8_t playerIdx, bool startInPlay)
    /*: gcLog(FlightRecorder::FlightRecorderLog::instance("GameController"))*/ {

    state.teamNr = teamNr;
    state.playerIdx = playerIdx;
    state.timeWhistleDetected = time_us() - 10_s;
    state.timeSwitchedToSet = time_us();
    state.timeSwitchedToReady = time_us();
    state.timeSwitchedToPlaying = time_us();

    LolaComm::setChestColor(RGB::BLACK);
    if (startInPlay) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        currentGcData.teams[0].messageBudget = 100;  // This is a hack since it doesn't count down.
        currentGcData.state = STATE_PLAYING;
        setLedColor(currentGcData.state);
    } else {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        currentGcData.teams[0].messageBudget = 100;  // We need some budget as default data. I know this is a hack...
    }
    currentGcData.teams[0].teamNumber = teamNr;
    currentGcData.teams[0].fieldPlayerColour = TEAM_BLUE;
    currentGcData.teams[1].teamNumber = 0;
    currentGcData.teams[1].fieldPlayerColour = TEAM_YELLOW;

    // gcOptionsLog = FlightRecorder::FlightRecorderLog::instance("GameController::OptionFramework");
    auto* gameControllerOptions = new OptionSet("GameController Options");

    // While this is an option in the options framework, we do not use its value.
    // We only care for its on-change handler as a trigger.
    gameControllerOptions->addOption(new BoolOption("ChestButton pressed", &ignored, [&](bool ignored) {
        // gcOptionsLog->infoMsg("Chest button pressed from NaoControl");
        LolaComm::fakeChestButtonPress();
    }));

    NaoControl::RobotOption::instance().addOptionSet(gameControllerOptions);
}

void GameControllerClient::setNewGameControllerData(const RoboCupGameControlData& data) {
    bool gameStateChange = false;
    uint8_t old_state = STATE_INITIAL;
    uint8_t new_state = STATE_INITIAL;

    // gcLog->debug(&data, sizeof(data), "GcPacket");
    bool team_color_changed = false;

    bool penalized_before, penalized_after;
    {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);

        const TeamInfo ownTeam_before =
                (currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1]);
        penalized_before = ownTeam_before.players[state.playerIdx].penalty != PENALTY_NONE;
        team_color_changed = data.teams[0].teamNumber != currentGcData.teams[0].teamNumber ||
                             data.teams[0].fieldPlayerColour != currentGcData.teams[0].fieldPlayerColour ||
                             data.teams[1].teamNumber != currentGcData.teams[1].teamNumber ||
                             data.teams[1].fieldPlayerColour != currentGcData.teams[1].fieldPlayerColour;
        currentGcData = data;

        for (auto& postprocess : postprocessor_callbacks) {
            std::tie(currentGcData, state) = postprocess->process(/*gcLog ,*/ state, currentGcData);
        }

        if (state.oldGameState != currentGcData.state) {
            gameStateChange = true;
            old_state = state.oldGameState;
            new_state = currentGcData.state;
            printf("GameController: GameState change from: %s to %s\n", gameStateToString(old_state).c_str(),
                   gameStateToString(new_state).c_str());
            state.oldGameState = currentGcData.state;
        }
        const TeamInfo& ownTeam_after =
                (currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1]);
        for (unsigned int i = 0; i < MAX_NUM_PLAYERS; i++)
            myTeamsPenaltyStatus[i] = (ownTeam_after.players[i].penalty != PENALTY_NONE);
        penalized_after = ownTeam_after.players[state.playerIdx].penalty != PENALTY_NONE;
        setLedColor(currentGcData.state);
    }

    if (gameStateChange)
        htwk::for_each(callbacks, [&](const auto& f) {
            f(old_state, new_state, currentGcData.setPlay, currentGcData.gamePhase, state.playerIdx);
        });
    if (!penalized_before && penalized_after) {
        penalty_start = time_us();
    }
    if (penalized_before && !penalized_after && penalty_start + 10_s < time_us()) {
        htwk::for_each(penalty_callbacks, [](const auto& f) { f(); });
    }
    if (team_color_changed) {
        const TeamInfo& own_team =
                currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1];
        const TeamInfo& opp_team =
                currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[1] : currentGcData.teams[0];
        htwk::for_each(team_color_callbacks, [&own_team, &opp_team](const auto& f) {
            f(own_team.teamNumber, own_team.fieldPlayerColour, opp_team.teamNumber, opp_team.fieldPlayerColour);
        });
    }
    if (data.gamePhase == GAME_PHASE_NORMAL && data.state == STATE_PLAYING) {
        int time_left = data.firstHalf * 600 + data.secsRemaining;
        const TeamInfo& own_team =
                currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1];
        htwk::for_each(message_limit_callbacks, [&time_left, &own_team](const auto& f) {
            f(time_left, own_team.messageBudget);
        });
    }
}

int64_t GameControllerClient::getWhistleTime() {
    std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
    return state.timeWhistleDetected;
}

void GameControllerClient::whistleDetected() {
    // gcLog->infoMsg("whistle detected");
    using namespace std::chrono_literals;

    int64_t now = time_us();
    auto deltaTime = now - state.timeWhistleDetected;

    if (deltaTime < 2_s)
        return;

    state.timeWhistleDetected = now;
    // gcLog->infoMsg("whistle detected %" PRId64, state.timeWhistleDetected);

    // Change immediately the playing state
    setNewGameControllerData(getGcData());
    // gcLog->infoMsg("finish whistle detected %d %lld %lld", oldGameState, timeSwitchedToSet, timeWhistleDetected);
}

std::string GameControllerClient::gameStateToString(uint8_t gamestate) {
    switch (gamestate) {
        case STATE_INITIAL:
            return "INITIAL";
        case STATE_READY:
            return "READY";
        case STATE_SET:
            return "SET";
        case STATE_PLAYING:
            return "PLAY";
        case STATE_FINISHED:
            return "FINISHED";
        default:
            return "UNKNOWN";
    }
}

void GameControllerClient::setLedColor(uint8_t gamestate) {
    RGB newLedColor;

    switch (gamestate) {
        case STATE_INITIAL:
            newLedColor = RGB::BLACK;
            break;
        case STATE_READY:
            newLedColor = RGB::BLUE;
            break;
        case STATE_SET:
            newLedColor = RGB::YELLOW;
            break;
        case STATE_PLAYING:
            newLedColor = RGB::GREEN;
            break;
        case STATE_FINISHED:
            newLedColor = RGB::BLACK;
            break;
    }
    newLedColor = (myTeamsPenaltyStatus[state.playerIdx] == PENALTY_NONE) ? newLedColor : RGB::RED;

    if (oldLedColor != newLedColor) {
        LolaComm::setChestColor(newLedColor);
        oldLedColor = newLedColor;
    }
}
