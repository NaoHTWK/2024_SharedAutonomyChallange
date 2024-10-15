#include "shared_autonomy_strategy.h"

#include <moveballorder.h>
#include <shootorder.h>
#include <worldmodel.pb.h>
#include <walktopositionorder.h>
#include <team_comm.h>

#include <memory>

using namespace htwk;
using namespace std;

SharedAutonomyStrategy::SharedAutonomyStrategy(uint8_t team_nr, PlayerIdx player_idx, WorldModel* world_model,
                                               GameControllerClient* gc_client/*, TeamComm* team_comm*/)
    : TeamStrategy(team_nr, player_idx, world_model, gc_client)/*,
      comm(team_comm)*/
    {

    printf("Initial Network Update; World Model");
    world_model->initialNetworkUpdate();
}

std::shared_ptr<Order> SharedAutonomyStrategy::play() {

    comm->sendEvent(TeamComm::Type::POSITION_UPDATE, true, __PRETTY_FUNCTION__, __LINE__);
    auto bots = world_model->getAliveNetworkRobots();

    if (bots.contains(2) && bots[2].last_packet_type == protobuf::worldmodel::TeamCommMsg::POSITION_UPDATE) {
        return std::make_shared<ShootOrder>(world_model->getNetworkRobots()[2].robot.pos.point(), STRENGTH::LOW);
    } else {
        return std::make_shared<WalkToPositionOrder>(world_model->getOwnRobot().pos, WalkToPositionOrder::Mode::SUPPORTER, false, HeadFocus::NOTHING);
    }
}
