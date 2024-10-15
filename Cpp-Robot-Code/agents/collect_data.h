#pragma once

#include <agent_base.h>
#include <worldmodel.h>

class CollectDataAgent : public AgentBase {
public:
    CollectDataAgent() : AgentBase("CollectDataAgent") {}
    MotionCommand proceed(std::shared_ptr<Order> order, WorldModel* world_model) override;
};
