#pragma once

#include <agent_base.h>
#include <worldmodel.h>
#include <walktopositionorder.h>
#include <shootorder.h>

class ShootAgent : public AgentBase {
public:
    ShootAgent() : AgentBase("Shoot") {}
    MotionCommand proceed(std::shared_ptr<Order> order, WorldModel* world_model) override;

private:
    enum class ShootStates { APPROACH, ALIGN, MICROALIGN, SHOOT};

    ShootStates shoot_state = ShootStates::APPROACH;

    int64_t time = 0;
    STRENGTH strength = STRENGTH::NORMAL;

    void checkState(Robot self, Position alignment);
    MotionCommand approach(const htwk::point_2d& ball, float target_dir);
    MotionCommand align(WorldModel *world_model, Position &alignment);
    MotionCommand microAlign(Position &alignment);
    MotionCommand shoot();
};
