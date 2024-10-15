#pragma once

#include <team_strategy.h>
#include <team_comm.h>
#include <point_2d.h>



class SharedManualStrategy : public TeamStrategy {

public:
    SharedManualStrategy(uint8_t team_nr, PlayerIdx player_idx, WorldModel* world_model, GameControllerClient* gc_client/*, htwk::TeamComm* team_comm*/);

protected:
    std::shared_ptr<Order> ready() override;

    std::shared_ptr<Order> set() override;

    std::shared_ptr<Order> play() override;

    std::shared_ptr<Order> setPlay_play() override;
    std::shared_ptr<Order> setPlay_ready() override;

private:
    void createOptionSet();
    htwk::TeamComm* comm;
    int button_pressed_autonom = 0;
    int button_pressed_manual = 0;

};



