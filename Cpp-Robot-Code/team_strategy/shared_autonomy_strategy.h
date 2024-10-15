#pragma once

#include <noorder.h>
#include <keepgoalorder.h>
//#include <penaltyshootorder.h>
#include <team_strategy.h>
#include <shootorder.h>
#include <memory>
#include <team_comm.h>


#include <point_2d.h>


#pragma once

#include <team_strategy.h>
#include <worldmodel.h>

class SharedAutonomyStrategy : public TeamStrategy {

public:
    SharedAutonomyStrategy(uint8_t team_nr, PlayerIdx player_idx, WorldModel* world_model, GameControllerClient* gc_client /*, htwk::TeamComm* team_comm*/);

protected:
    std::shared_ptr<Order> ready() override {
        return std::make_shared<NoOrder>();
    }

    std::shared_ptr<Order> set() override {
        return std::make_shared<NoOrder>();
    }

    std::shared_ptr<Order> play() override;

    std::shared_ptr<Order> setPlay_play() override {
        return std::make_shared<NoOrder>();
    }
    std::shared_ptr<Order> setPlay_ready() override {
        return std::make_shared<NoOrder>();
    }

private:
    htwk::TeamComm* comm = nullptr;

};
