// Created by lennart on 7/16/24.

#include "shared_manual_strategy.h"
#include <walktopositionorder.h>
#include <position.h>
#include <set>
#include <algorithm_ext.h>
#include <stl_ext.h>
#include <noorder.h>
#include <robotoption.h>
#include <optionset.h>
#include <booloption.h>
#include <intoption.h>

#include <moveballorder.h>
#include <shootorder.h>



using namespace htwk;
using namespace std;


//using TeamCommMsg = protobuf::worldmodel::TeamCommMsg;

SharedManualStrategy::SharedManualStrategy(uint8_t team_nr, PlayerIdx player_idx, WorldModel* world_model,
                                           GameControllerClient* gc_client/*, TeamComm* team_comm*/)

    : TeamStrategy(team_nr, player_idx, world_model, gc_client)/*,
      comm(team_comm) */{
    gc_client->addMessageLimitCallback(
            std::bind(&TeamComm::updateGcData, comm, std::placeholders::_1, std::placeholders::_2));
    auto* manualStrategyOptions = new NaoControl::OptionSet("manual_strategy");
    manualStrategyOptions->addOption(new NaoControl::IntOption("button_pressed_autonom", &button_pressed_autonom));
    manualStrategyOptions->addOption(new NaoControl::IntOption("button_pressed_manual", &button_pressed_manual));

    world_model->initialNetworkUpdate();


    NaoControl::RobotOption::instance().addOptionSet(manualStrategyOptions);
	printf("NaoControl RobotOption is added....");

}



std::shared_ptr<Order> SharedManualStrategy::ready() {
    // Implementation of the ready method
    printf("SEND TeamComm message");
    comm->sendEvent(TeamComm::Type::POSITION_UPDATE, true, __PRETTY_FUNCTION__, __LINE__);
    // The missing return value here is an error in the code. But the code was used like this during the Shared Autonomy Challenge during the competition. So we publish it as it is.
}

std::shared_ptr<Order> SharedManualStrategy::set() {
    // Implementation of the set method
    return std::make_shared<NoOrder>();
}

std::shared_ptr<Order> SharedManualStrategy::play() {
    // Implementation of the play method

    if (button_pressed_autonom) {
        std::cout << "AUTONOM button pressed" << std::endl;
        comm->sendEvent(TeamComm::Type::POSITION_UPDATE, true, __PRETTY_FUNCTION__, __LINE__);
        //auto p = robots.at(2).robot.pos().point();
        //auto p = world_model->getNetworkRobots()[2].robot.pos;
        //printf("%.2f, %.2f, %.2f\n", p.x, p.y, p.a);
        //return make_shared<MoveBallOrder>(p.x, p.y));

    } else {
        comm->sendEvent(TeamComm::Type::NEW_STRIKER, true, __PRETTY_FUNCTION__, __LINE__);
    }

    if (button_pressed_manual) {
        std::cout << "MANUAL button pressed" << std::endl;
        auto bots = world_model->getAliveNetworkRobots();
        int idx = -1;
        if (bots.contains(0)) {
            idx = 0;
        } else if (bots.contains(1)) {
            idx = 1;
        }
        if (idx == -1) {
            return std::make_shared<NoOrder>();
        }
        std::cout << idx << std::endl;
        return std::make_shared<ShootOrder>(bots[idx].pos.point(), STRENGTH::LOW);
    }


    return std::make_shared<NoOrder>();


}


std::shared_ptr<Order> SharedManualStrategy::setPlay_ready() {
    // Implementation of the setPlay_ready method
    return std::make_shared<NoOrder>();
}

std::shared_ptr<Order> SharedManualStrategy::setPlay_play() {
    // Implementation of the setPlay_play method
    return std::make_shared<NoOrder>();
}
