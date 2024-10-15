#pragma once

#include <cam_constants.h>
#include <imu.h>
#include <joints.h>
#include <motion_command.h>
#include <point_2d.h>
#include <sensor_data.h>
#include <worldmodel.h>

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>

class HeadControl {
public:
    using Focus = HeadFocus;
    struct sta_settings {
        float w_fac = 0;
        float t = 0;
        float gain = 0;
        sta_settings() {}
        sta_settings(float w_fac, float t, float gain) : w_fac(w_fac), t(t), gain(gain) {}
    };

    HeadControl(WorldModel* world_model);

    void setFocus(Focus new_focus, bool may_move_) {
        focus.store(new_focus);
        may_move.store(may_move_);
    }
    void updateDetectedBall(const htwk::point_2d& ball, int64_t img_time) {
        std::lock_guard<std::mutex> lck(ball_detection_mtx);
        HeadJointAngles angles = SensorData::instance().getHead(img_time);
        ball_pos = YawPitch{angles[0] - std::atan((ball.x - cam_width / 2.f) / cam_depth),
                            angles[1] + std::atan((ball.y - cam_height / 2.f) / cam_depth)};
        last_ball_percept = img_time;
    }
    static float smoothTriAng(float w, sta_settings s);
    static float smoothTriAngStartTime(YawPitch head_pos, sta_settings sta_yaw,
                                       sta_settings sta_pitch = sta_settings());

private:
    void proceed(int64_t time);

    std::atomic<Focus> focus = Focus::NOTHING;
    std::atomic_bool may_move = false;
    std::mutex ball_detection_mtx;
    int64_t last_ball_percept = 0;
    YawPitch ball_pos;
    WorldModel* world_model;
    YawPitch head_pos{0, 0};
    Focus last_focus = Focus::NOTHING;
    int64_t last_time = 0;
    float time_step = 0;
    // Initialized with true to force time_step calculation in Focus::BALL.
    bool ball_found = true;
    float pitch_base_value = 0.365f; // if in doubt 0.375 was 0.5 in the beginning
};
