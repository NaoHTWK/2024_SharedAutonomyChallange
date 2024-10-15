#include "keyframe_controller.h"

#include <iostream>

using namespace std;

KeyframeController::KeyframeController() {
    for (const auto& [motion, file] : motion_filenames)
        motions.emplace(motion, Motion(file));
}

Joints KeyframeController::proceed(const AnkleBalancer& ankle_balancer) {
    Joints joints;
    if (!motion_to_play){
        cerr << "KeyframeController called unnecessarily! (This is really bad, fix it!)" << endl;
        set_stiffness(-1.f, &joints.arms);
        set_stiffness(-1.f, &joints.legs);
        set_stiffness(-1.f, &joints.head);
        return joints;
    }
    joints = motions[*motion_to_play].motion[cur_frame];
    if (!motions[*motion_to_play].stabilize.empty() && motions[*motion_to_play].stabilize[cur_frame]){
        joints.legs[LAnklePitch].angle += ankle_balancer.pitch * 0.5f;
        joints.legs[RAnklePitch].angle += ankle_balancer.pitch * 0.5f;
    }
    cur_frame++;
    if (cur_frame == motions[*motion_to_play].motion.size())
        motion_to_play = {};
    return joints;
}
