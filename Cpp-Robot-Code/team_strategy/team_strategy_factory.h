#pragma once

#include <team_comm.h>
#include <team_strategy.h>
#include <worldmodel.h>

#include <string>

class TeamStrategyFactory {
public:
    static TeamStrategy* create(const std::string& strategyName, uint8_t team_nr, PlayerIdx player_idx,
                                WorldModel* world_model, GameControllerClient* gc_client/*, htwk::TeamComm* team_comm*/);
};
