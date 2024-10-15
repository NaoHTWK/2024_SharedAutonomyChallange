#include "walk_tuner.h"

#include <intoption.h>
#include <leds.h>
#include <lola_comm.h>
#include <moveballgoalorder.h>
#include <moveballorder.h>
#include <soccerfield.h>
#include <walktopositionorder.h>
#include <option.h>
#include <optionset.h>
#include <floatoption.h>
#include <robotoption.h>
#include <point_2d.h>
#include <localization_utils.h>
#include <string>


using namespace std;
using namespace htwk;
using namespace NaoControl;

WalkTuner::WalkTuner() : AgentBase("WalkTuner") {
    auto* walkTunerOptions = new NaoControl::OptionSet("walktuner");
    walkTunerOptions->addOption(new FloatOption("vx", &rel_vx, -0.2f, 0.27f, 0.005f));
    walkTunerOptions->addOption(new FloatOption("vy", &rel_vy, -0.35f, 0.35f, 0.005f));
    walkTunerOptions->addOption(new FloatOption("va", &rel_va, -1.7f, 1.7f, 0.005f));
    walkTunerOptions->addOption(new FloatOption("shoot", &shoot, -1.f, 1.f, 1.0f));

    walkTunerOptions->addOption(
            new FloatOption("abs_x", &abs_x, -SoccerField::width() / 2, SoccerField::width() / 2, 0.005f));
    walkTunerOptions->addOption(
            new FloatOption("abs_y", &abs_y, -SoccerField::length() / 2, SoccerField::length() / 2, 0.005f));
    walkTunerOptions->addOption(new FloatOption("abs_a", &abs_a, -1.7f, 1.7f, 0.005f));
    walkTunerOptions->addOption(new FloatOption("abs_allowed_delta", &abs_allowed_delta, -1.f, 1.f, 0.005f));

    walkTunerOptions->addOption(new IntOption<int8_t>("mode", &mode, 0, 3, 1));

    NaoControl::RobotOption::instance().addOptionSet(walkTunerOptions);
}

Shoot is_shoot_effective(const std::optional<RelativeBall>& ball) {
    if (!ball) return Shoot::NONE;
    if (ball->lastSeenTime < time_us() - 1_s) return Shoot::NONE;
    float ball_x_pos = 0.30f;
    printf("in is_shoot_effective");

    if (ball->pos.x > ball_x_pos || std::abs(ball->pos.y) > 0.15f || std::abs(ball->pos.y) < 0.01f) {
        return Shoot::NONE;
    }
    if (ball->pos.y < 0) {
        return Shoot::RIGHT;
    } else {
        return Shoot::LEFT;
    }
}

MotionCommand WalkTuner::proceed(std::shared_ptr<Order> order, WorldModel* wm) {
    Robot ownRobot = wm->getOwnRobot();
    static bool printed_error = false;

    if (old_mode != mode) {
        printf("Switching walktuner mode from '%s' to '%s'\n", walk_mode_to_string(old_mode).c_str(),
               walk_mode_to_string(mode).c_str());
        old_mode = mode;
    }

    switch (mode) {
        case 0:
            printed_error = false;
            LolaComm::forceSit(false);
            return MotionCommand::Stand();
        case 1:
            printed_error = false;
            return walk_relative(wm);
        case 2:
            printed_error = false;
            return walk_to_absolute_position(wm);
        case 3:
            printed_error = false;
            LolaComm::forceSit(true);
            return MotionCommand::Stand();
        case 4:
            printed_error = true;
            printf("In Case 4, walk_relative");
            return walk_relative(wm, true);
		case 5:
		    printed_error = false;
            printf("In Case 5, walk_relative");

            return walk_relative(wm, true);
    }

    if (!printed_error) {
        fprintf(stderr, "Unknown mode. Stand still!\n");
        printed_error = true;
    }

    return MotionCommand::Stand();
}

MotionCommand WalkTuner::walk_relative(WorldModel* wm, bool shoot_requested) {
    Shoot shoot_allowed = is_shoot_effective(wm->getOwnRobot().ball);
    return MotionCommand::Walk(WalkRequest{rel_vx, rel_vy, rel_va, shoot_requested ? shoot_allowed : Shoot::NONE},
                               HeadFocus::KEEP_STILL);
}

MotionCommand WalkTuner::walk_to_absolute_position(WorldModel* wm) {
    Position dest{abs_x, abs_y, abs_a};
    Position robot_pos = wm->getOwnRobot().pos;
    point_2d dest_rel = LocalizationUtils::absToRel(dest, robot_pos);

    if (wm->getOwnRobot().loc_quality < 0.6)
        return MotionCommand::Nothing;

    if(point_2d{robot_pos}.dist(dest) <= abs_allowed_delta) {
        mode = 0;
        return MotionCommand::Stand(HeadFocus::KEEP_STILL);
    }

    return MotionCommand::Walk({clamp(dest_rel.x * 1.f, -0.15f, 0.22f), clamp(dest_rel.y * 1.f, -0.3f, 0.3f),
                                clamp(normalizeRotation(abs_a - robot_pos.a) * 2.f, -0.8f, 0.8f)},
                               HeadFocus::KEEP_STILL);
}

string WalkTuner::walk_mode_to_string(int8_t walk_mode) {
    switch (walk_mode) {
        case 0:
            return "stand";
        case 1:
            return "relative";
        case 2:
            return "absolute";
    }

    return "unknown";
}
