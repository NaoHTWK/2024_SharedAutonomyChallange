#pragma once

#include <imu.h>
#include <joints.h>
#include <motion.h>
//#include <walking_engine.h>
#include <ankle_balancer.h>
#include <map>
#include <optional>
#include <string>

enum class KeyframeMotionID { GOALIE_LEFT, GOALIE_RIGHT, GOALIE_CENTER, SHOOT_LEFT, SHOOT_RIGHT, DANCE, SHOOT_RIGHT_100 };

class KeyframeController {
public:
    KeyframeController();

    bool shouldRun() {
        return static_cast<bool>(motion_to_play);
    }
    Joints proceed(const AnkleBalancer& ankle_balancer);
    void playMotion(KeyframeMotionID id) {
        if (motion_to_play && id == *motion_to_play)
            return;
        motion_to_play = id;
        cur_frame = 0;
    }
    // Whether we should tell everyone upstream that we're "fallen" so that e.g. vision doesn't run.
    // For goalie motions we want to restart vision processing at the last part of the get-up to check whether there's a
    // ball right in front of our feet.
    bool consideredFallen() {
        if (!shouldRun())
            return false;
        if (motion_to_play == KeyframeMotionID::GOALIE_LEFT || motion_to_play == KeyframeMotionID::GOALIE_RIGHT ||
            motion_to_play == KeyframeMotionID::GOALIE_CENTER) {
            if (cur_frame >= motions[*motion_to_play].motion.size() - 40) {
                return false;
            }
        }
        return true;
    }

private:
    std::map<KeyframeMotionID, Motion> motions;
    std::map<KeyframeMotionID, std::string> motion_filenames{{KeyframeMotionID::GOALIE_LEFT, "goalie_left.mot"},
                                                             {KeyframeMotionID::GOALIE_RIGHT, "goalie_right.mot"},
                                                             {KeyframeMotionID::GOALIE_CENTER, "goalie_center.mot"},
                                                             {KeyframeMotionID::SHOOT_LEFT, "shoot_left.mot"},
                                                             {KeyframeMotionID::SHOOT_RIGHT, "shoot_right.mot"},
                                                             {KeyframeMotionID::DANCE, "dance.mot"},
                                                             {KeyframeMotionID::SHOOT_RIGHT_100, "shoot_right_100.mot"}};
    std::optional<KeyframeMotionID> motion_to_play;
    size_t cur_frame = 0;
};
