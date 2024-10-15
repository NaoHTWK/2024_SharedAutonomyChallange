#pragma once

#include <RoboCupGameControlData.h>
#include <envinfo.h>
#include <gamecontrollerclient.h>
#include <order.h>
#include <worldmodel.h>

#include <memory>
#include <vector>

class TeamStrategy {
public:
    TeamStrategy(uint8_t team_nr, PlayerIdx player_idx, WorldModel* world_model, GameControllerClient* gc_client)
        : team_nr(team_nr), player_idx(player_idx), world_model(world_model), gc_client(gc_client) {}
    virtual ~TeamStrategy() = default;

    TeamStrategy(TeamStrategy&) = delete;
    TeamStrategy(TeamStrategy&&) = delete;
    TeamStrategy& operator=(const TeamStrategy&) = delete;
    TeamStrategy& operator=(TeamStrategy&&) = delete;

    virtual std::shared_ptr<Order> proceed(RoboCupGameControlData gc_data_) final;

protected:
    virtual bool isOurKickoff() final {
        return gc_data.kickingTeam == team_nr;
    }

    virtual std::shared_ptr<Order> ready() = 0;
    virtual std::shared_ptr<Order> set() = 0;
    virtual std::shared_ptr<Order> play() = 0;
    virtual std::shared_ptr<Order> setPlay_ready() = 0;
    virtual std::shared_ptr<Order> setPlay_play() = 0;

    const uint8_t team_nr;
    const PlayerIdx player_idx;

    int64_t time_us_lastKickoff = time_us();
    int64_t time_us_inPlay = time_us();

    RoboCupGameControlData gc_data{};
    WorldModel* world_model{};
    GameControllerClient* gc_client;

    std::map<PlayerIdx, Robot> robots;
    std::map<PlayerIdx, Robot> last_robots;

    uint8_t prev_state = STATE_INITIAL;
    uint8_t prev_setPlay = SET_PLAY_NONE;
};
