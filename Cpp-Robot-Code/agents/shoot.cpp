#include <localization_utils.h>
#include <moveballgoalorder.h>
#include <penaltyshootorder.h>
#include <point_3d.h>
#include <position.h>
#include <shoot.h>
#include <shootorder.h>

#include <cmath>

using namespace htwk;
using namespace std;


MotionCommand ShootAgent::approach(const point_2d& ball, float target_dir) {
    // TODO: include target_dir

    target_dir = clamp(target_dir, -90_deg, 90_deg);

    constexpr float dist_behind_ball = 0.20f;
    point_2d dest_vec = point_2d(1, 0).rotated(target_dir) * dist_behind_ball;
    point_2d target1 = ball - dest_vec;

    point_3d motion;
    float dist_factor = max(0.f, 0.5f - target1.norm()) * 2.f;
    float shift_y_to_target = deadband_filter(-30_deg, target1.to_direction(), 30_deg);
    float good_angle =
            clamp((30_deg - abs(deadband_filter(-60_deg, target1.to_direction(), 60_deg))) / 30_deg, 0.f, 1.f);
    motion.x = clamp((target1.x + 0.4f) * good_angle, 0.f, 0.35f);
    motion.y = clamp(target1.to_direction() * 0.2f * dist_factor + shift_y_to_target, -0.38f, 0.38f);
    motion.z = clamp((target1 + point_2d(0.2f, 0)).to_direction() * 1.2f, -1.7f, 1.7f);
    return MotionCommand::Walk(WalkRequest{motion.x, motion.y, motion.z}, HeadFocus::BALL);
}

MotionCommand ShootAgent::align(WorldModel *world_model, Position &alignment) {
    point_2d ball = world_model->getOwnRobot().ball->pos;

    return MotionCommand::Walk({
                                       clamp((ball.x - .2f) * 3.f, -.2f, .2f),
                                       clamp(alignment.y * 3.f, -.3f, .3f),
                                       clamped_linear_interpolation(
                                               abs(alignment.a),
                                               0.2f,
                                               1.5f,
                                               0,
                                               1.5f
                                               ) * sgn(alignment.a)
                               },
                               HeadFocus::BALL);
}

MotionCommand ShootAgent::microAlign(Position &alignment) {
    return MotionCommand::Walk(
            WalkRequest{
                    clamped_linear_interpolation(
                            alignment.x,
                            -.05f,
                            .01f,
                            -.01f,
                            .01f
                            ),
                    clamped_linear_interpolation(
                            abs(alignment.y),
                            .05f,
                            .1f,
                            .0f,
                            .08f
                            ) * sgn(alignment.y),

                    clamped_linear_interpolation(
                            abs(alignment.a),
                            .1f,
                            .3f,
                            .0f,
                            .08f
                            ) * sgn(alignment.a),
            },
            HeadFocus::BALL);
}

MotionCommand ShootAgent::shoot() {
    if (time == 0)
        time = time_us();

    if (time_us() - time < 1000_ms)
        return MotionCommand::Nothing;

    time = 0;
    if(strength == STRENGTH::LOW) {
        return MotionCommand::PlayMotion(KeyframeMotionID::SHOOT_RIGHT_100);
    }
    return MotionCommand::PlayMotion(KeyframeMotionID::SHOOT_RIGHT);
}

void ShootAgent::checkState(Robot self, Position alignment) {
    float approach_dist = .5f;

    float align_dist = .4f;

    float microalign_tolerance_x = .1f;
    float miroalign_tolerance_a = 5_deg;

    float shoot_tolerance_x = .02f;
    float shoot_tolerance_y = .01f;
    float shoot_tolerance_a = 2_deg;


    if (self.ball->pos.norm() > approach_dist) {
        shoot_state = ShootStates::APPROACH;
    } else if (self.ball->pos.norm() <= align_dist && shoot_state == ShootStates::APPROACH) {
        shoot_state = ShootStates::ALIGN;
    } else if (within(alignment.x, -microalign_tolerance_x, microalign_tolerance_x)
               && within(alignment.a, -miroalign_tolerance_a, miroalign_tolerance_a)
               && shoot_state == ShootStates::ALIGN) {
        shoot_state = ShootStates::MICROALIGN;

    } else if (within(alignment.x, -shoot_tolerance_x, shoot_tolerance_x)
               && within(alignment.y, -shoot_tolerance_y, shoot_tolerance_y)
               && within(alignment.a, -shoot_tolerance_a, shoot_tolerance_a)
               && shoot_state == ShootStates::MICROALIGN) {
        shoot_state = ShootStates::SHOOT;
    } else if (self.ball->pos.norm() > (align_dist - .2f)
               && self.ball->pos.norm() < (align_dist - .1f) && shoot_state == ShootStates::SHOOT) {
        shoot_state = ShootStates::MICROALIGN;
    } else if (self.ball->pos.norm() >= (align_dist - .1f)
               && (shoot_state == ShootStates::SHOOT || shoot_state == ShootStates::MICROALIGN)) {
        shoot_state = ShootStates::ALIGN;
    }
}

MotionCommand ShootAgent::proceed(std::shared_ptr<Order> order, WorldModel *world_model) {
    if (!(isOrder<PenaltyShootOrder>(order) || isOrder<ShootOrder>(order)))
        return MotionCommand::Nothing;
    std::cout<<"shoooooooooooot"<<std::endl;
    Robot self = world_model->getOwnRobot();
    strength = static_pointer_cast<ShootOrder>(order)->getStrength();

    if (self.ball) {
        float target_alignment_offset = .155f;
        Position alignment;
        point_2d rel_pos_target;
        if (isOrder<PenaltyShootOrder>(order)){
            rel_pos_target = {-1, 0};
            alignment = {self.ball->pos.x + target_alignment_offset, self.ball->pos.y, 0.f};
        }
        if (isOrder<ShootOrder>(order)) {
            float foot_offset_y = .054f;
            rel_pos_target =
                    LocalizationUtils::absToRel(static_pointer_cast<ShootOrder>(order)->getTarget(), self.pos);
            point_2d ball_to_target = (rel_pos_target - self.ball->pos).normalized();
            //point_2d foot_offset_direction {1, -(ball_to_target.x / ball_to_target.y)}; // y= (x1*x2)/y1, x2=1
            //foot_offset_direction = foot_offset_direction.normalized();

            alignment = {self.ball->pos - target_alignment_offset * ball_to_target, ball_to_target.to_direction()};
            alignment += Position{0,foot_offset_y,0};
        }

        ShootAgent::checkState(self, alignment);

        if (shoot_state == ShootStates::APPROACH) {
            //printf("APPROACH\n");
            return this->approach(self.ball->pos, rel_pos_target.to_direction());
        } else if (shoot_state == ShootStates::ALIGN) {
            //printf("ALIGN\n");
            return this->align(world_model, alignment);
        } else if (shoot_state == ShootStates::MICROALIGN) {
            //printf("MICROALIGN\n");
            return this->microAlign(alignment);
        } else if (shoot_state == ShootStates::SHOOT) {
            //printf("SHOOT\n");
            return this->shoot();
        }
    }
    return MotionCommand::Walk(WalkRequest{0.f, 0.f, 0.f}, HeadFocus::BALL);
}
