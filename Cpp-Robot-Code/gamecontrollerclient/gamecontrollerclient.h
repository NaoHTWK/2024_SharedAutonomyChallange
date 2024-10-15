#ifndef GAMECONTROLLERCLIENT_H
#define GAMECONTROLLERCLIENT_H

#include <RoboCupGameControlData.h>
//#include <flightrecorderlog.h>
#include <leds.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

struct GameControllerClientState {
    uint8_t teamNr;
    uint8_t playerIdx;
    int64_t timeWhistleDetected;
    int64_t timeSwitchedToSet;
    int64_t timeSwitchedToPlaying;
    int64_t timeSwitchedToReady;
    uint8_t oldGameState = STATE_INITIAL;
};

/**
 * @brief The GameControllerClientPostprocessor class is a postprocessor to overwrite the gc state based on other input.
 * It can be registered to the GameControllerClient.
 */
class GameControllerClientPostprocessor {
public:
    virtual ~GameControllerClientPostprocessor() = default;
    virtual std::tuple<RoboCupGameControlData, GameControllerClientState> process(
            /*FlightRecorder::LogPtr gcLog,*/ GameControllerClientState state, RoboCupGameControlData newGcState) = 0;
};

class GameControllerClient {
public:
    GameControllerClient(uint8_t teamNr, uint8_t playerIdx, bool startInPlay);

    void setNewGameControllerData(const RoboCupGameControlData& data);

    uint8_t getCurrentState() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        return currentGcData.state;
    }

    bool amIPenalized() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        return myTeamsPenaltyStatus[state.playerIdx];
    }

    bool amIAllowedToWalk() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        if (myTeamsPenaltyStatus[state.playerIdx])
            return false;
        return currentGcData.state == STATE_READY || currentGcData.state == STATE_PLAYING;
    }

    bool amIAllowedToMoveHead() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        if (myTeamsPenaltyStatus[state.playerIdx])
            return false;
        return currentGcData.state == STATE_READY || currentGcData.state == STATE_SET ||
               currentGcData.state == STATE_PLAYING;
    }

    std::vector<bool> getMyTeamsPenaltyStatus() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        return myTeamsPenaltyStatus;
    }

    static std::vector<bool> getMyTeamsPenaltyStatus(const RoboCupGameControlData& data, int team_id) {
        const TeamInfo& own = (data.teams[0].teamNumber == team_id ? data.teams[0] : data.teams[1]);
        std::vector<bool> myTeamsPenaltyStatus(MAX_NUM_PLAYERS, false);
        for (unsigned int i = 0; i < MAX_NUM_PLAYERS; i++)
            myTeamsPenaltyStatus[i] = (own.players[i].penalty != PENALTY_NONE);
        return myTeamsPenaltyStatus;
    }

    void addGameStateCallback(const std::function<void(uint8_t oldState, uint8_t newState, uint8_t setPlay,
                                                       uint8_t gamePhase, uint8_t playerIdx)>& callback) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        callbacks.push_back(callback);
    }

    void addPenaltyCallback(const std::function<void()>& callback) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        penalty_callbacks.push_back(callback);
    }

    void addTeamColorCallback(const std::function<void(uint8_t own_team_id, uint8_t own_team_color, uint8_t opp_team_id,
                                                       uint8_t opp_team_color)>& callback) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        team_color_callbacks.push_back(callback);
        const TeamInfo& own_team =
                currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1];
        const TeamInfo& opp_team =
                currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[1] : currentGcData.teams[0];
        // TODO: add goalie color
        callback(own_team.teamNumber, own_team.fieldPlayerColour, opp_team.teamNumber, opp_team.fieldPlayerColour);
    }

    void addMessageLimitCallback(const std::function<void(int time_left, int messages_left)>& callback) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        message_limit_callbacks.push_back(callback);
    }

    void addPostprocessor(std::shared_ptr<GameControllerClientPostprocessor> postprocessor) {
        std::unique_lock<std::shared_timed_mutex> lock(gcMutex);
        postprocessor_callbacks.push_back(postprocessor);
    }

    TeamInfo getOwnTeam() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        return (currentGcData.teams[0].teamNumber == state.teamNr ? currentGcData.teams[0] : currentGcData.teams[1]);
    }

    RoboCupGameControlData getGcData() {
        std::shared_lock<std::shared_timed_mutex> lock(gcMutex);
        return currentGcData;
    }

    void whistleDetected();
    int64_t getWhistleTime();

private:
    using clock = std::chrono::system_clock;
    std::shared_timed_mutex gcMutex;
    RoboCupGameControlData currentGcData{.state = STATE_INITIAL};
    GameControllerClientState state;

    //FlightRecorder::LogPtr gcOptionsLog;
    RGB oldLedColor;
    std::vector<std::function<void(uint8_t oldState, uint8_t newState, uint8_t setPlay, uint8_t gamePhase,
                                   uint8_t playerIdx)>>
            callbacks;
    std::vector<std::function<void()>> penalty_callbacks;
    std::vector<std::function<void(uint8_t own_team_id, uint8_t own_team_color, uint8_t opp_team_id,
                                   uint8_t opp_team_color)>>
            team_color_callbacks;
    std::vector<std::function<void(int, int)>> message_limit_callbacks;
    std::vector<std::shared_ptr<GameControllerClientPostprocessor>> postprocessor_callbacks;
    int64_t penalty_start = 0;
    std::vector<bool> myTeamsPenaltyStatus = std::vector<bool>(MAX_NUM_PLAYERS, false);
    //FlightRecorder::LogPtr gcLog;
    std::string gameStateToString(uint8_t gamestate);
    void setLedColor(uint8_t gamestate);
};

#endif
