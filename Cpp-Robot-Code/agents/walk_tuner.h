#pragma once

#include <agent_base.h>
#include <joints.h>

class WalkTuner : public AgentBase {
public:
    WalkTuner();
    MotionCommand proceed(std::shared_ptr<Order> order, WorldModel* wm) override;

private:
    MotionCommand walk_relative(WorldModel* wm, bool shoot_requested = false);
    MotionCommand walk_to_absolute_position(WorldModel* wm);

    float abs_x=0, abs_y=0, abs_a=0, abs_allowed_delta=0.1;
    float rel_vx=0.01,rel_vy=0,rel_va=0;
    float shoot=0;
    int8_t mode = 0; // We support 0 = stand, 1 = relative movement, 2 = walk to a abs position
    int8_t old_mode = 0;

    std::string walk_mode_to_string(int8_t walk_mode);
};
