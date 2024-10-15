#pragma once

#include <RoboCupGameControlData.h>
// #include <flightrecorderlog.h>
#include <google/protobuf/message_lite.h>
#include <stl_ext.h>
#include <worldmodel.h>
#include <worldmodel.pb.h>

#include <functional>
#include <map>
#include <memory>
#include <thread>

namespace htwk {

class TeamComm {
public:
    // Keep in sync with worldmodel.proto!!!
    enum class Type { NONE, STRIKER, GOALIE, NEW_STRIKER, POSITION_UPDATE, OPPONENT_UPDATE, NEW_STRIKER_MOVE, SWITCH_TO_READY, PASS_READY};

    TeamComm(int team_id, WorldModel* world_model) : team_id(team_id), world_model(world_model) {
        send_thread = launch_named_thread("TeamCommSender", false, std::mem_fn(&TeamComm::continuousSender), this);
        recv_thread = launch_named_thread("TeamCommReceiver", false, std::mem_fn(&TeamComm::receive), this);
    }

    void setContinuous(Type type) {
        if (type != continuous_type) {
            //log->infoMsg("new continuous type: %d, current frequency: %f", (int)type, getContinuousDuration());
            last_continuous_timestamp = time_us();
        }
        continuous_type = type;
    }
    bool sendEvent(Type type, bool important, const char* sender_func_name, uint32_t line_number);

    bool budgetAvailable() {
        return remaining_message_budget > hard_safety;
    }

    std::vector<PlayerIdx> notify_newStriker();
    bool notify_newPositionUpdate();
    bool notify_switchToReady();

    void updateGcData(int time_left, int messages_left) {
        remaining_game_time = time_left;
        remaining_message_budget = messages_left;
    }
    void updateGameState(uint8_t /*old_state*/, uint8_t new_state, uint8_t /*set_play*/, uint8_t game_phase,
                         uint8_t /*player_idx*/) {
        should_send_continuous =
                new_state == STATE_PLAYING && (game_phase == GAME_PHASE_NORMAL || game_phase == GAME_PHASE_OVERTIME);
    }

    float getContinuousDuration() {
        float frac_time_remaining = remaining_game_time / (float)total_game_time;
        float continuous_messages_remaining =
                remaining_message_budget - soft_safety - est_event_messages * frac_time_remaining;
        if (continuous_messages_remaining <= 0)
            return std::numeric_limits<float>::infinity();
        return std::max(min_continuous_secs, remaining_game_time / continuous_messages_remaining);
    }

private:
    // Estimated event messages per half:
    //   20 striker changes
    //     1 new striker
    //     2 supporters
    //   3 ready phases
    //     5 initial positions
    //     2 opponent observations
    static constexpr int est_event_messages = 600;
    const std::map<Type, float /*ratio*/> continuous_message_ratios{{Type::STRIKER, 1.f}};//, {Type::GOALIE, 0.25f}};
    static constexpr int total_game_time = 10 * 60 * 2;
    static constexpr int total_message_budget = 1200;
    static constexpr int soft_safety = 30;
    static constexpr int hard_safety = 30;
    static constexpr float min_continuous_secs = 2.f;
    static constexpr int max_message_size = 128;

    void continuousSender();
    void receive();

    //FlightRecorder::LogPtr log = FlightRecorder::FlightRecorderLog::instance("TeamComm");
    int team_id;
    WorldModel* world_model;
    std::thread send_thread;
    std::thread recv_thread;
    int remaining_game_time = total_game_time;
    int remaining_message_budget = total_message_budget;

    Type continuous_type = Type::NONE;
    int64_t last_continuous_timestamp = 0;
    bool should_send_continuous = false;

    std::mutex mtx;
    std::vector<PlayerIdx> new_striker_ids;
    bool is_newPositionUpdate_event = false;
    int64_t last_event_us = 0;

    bool is_newSwitchReady_event = false;
};

}  // namespace htwk
