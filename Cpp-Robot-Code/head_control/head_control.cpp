#include <async.h>
#include <head_control.h>
#include <lola_comm.h>
#include <robotoption.h>
#include <stl_ext.h>
#include <optionset.h>
#include <localization_utils.h>

using namespace NaoControl;
using namespace htwk;
using namespace std;

/**
 * @param w current position
 * @param t slowdown (higher value slows more)
 * @param gain maximum yaw-angle of head
 */
float HeadControl::smoothTriAng(float w, sta_settings s) {
    // TODO: Replace this monster with something understandable. Do we even want to break? Why not linear
    // acceleration/deceleration?
    w *= s.w_fac;
    while (w < 0)
        w += M_PIf * 2.f;
    while (w > M_PIf * 2.f)
        w -= M_PIf * 2.f;
    if ((w > s.t && w < M_PIf - s.t) || (w > M_PIf + s.t && w < M_PIf * 2.f - s.t)) {
        if (w < M_PIf) {
            return s.gain * (w / M_PIf * 2.f - 1);
        } else {
            return s.gain * ((M_PIf * 2.f - w) / M_PIf * 2.f - 1.f);
        }
    } else {
        float r = s.t;
        float a = 1.f / 2.f / r;
        float b = -r / 2.f + s.t;
        if (w <= s.t) {
            float x = w;
            return s.gain * ((a * x * x + b) / M_PIf * 2.f - 1.f);
        } else if (w <= M_PIf) {
            float x = M_PIf - w;
            return s.gain * ((1.f - (a * x * x + b) / M_PIf) * 2.f - 1.f);
        } else if (w >= M_PIf * 2.f - s.t) {
            float x = w - M_PIf * 2.f;
            return s.gain * ((a * x * x + b) / M_PIf * 2.f - 1.f);
        } else if (w > M_PIf) {
            float x = w - M_PIf;
            return s.gain * ((1.f - (a * x * x + b) / M_PIf) * 2.f - 1.f);
        }
        return 0;
    }
}

float HeadControl::smoothTriAngStartTime(YawPitch head_pos, sta_settings sta_yaw, sta_settings sta_pitch) {
    float min_w = 0;
    float min_dist = std::numeric_limits<float>::infinity();
    for (float w = 0; w < M_PIf * 2.f / std::abs(sta_yaw.w_fac); w += 0.1f / std::abs(sta_yaw.w_fac)) {
        float dist = 0;
        if (sta_pitch.w_fac == 0) {
            dist = std::abs(smoothTriAng(w, sta_yaw) - head_pos.yaw);
        } else {
            // TODO: This doesn't work if abs(w_fac) is different in sta_yaw vs. sta_pitch.
            dist = (htwk::point_2d(smoothTriAng(w, sta_yaw), smoothTriAng(w, sta_pitch)) -
                    htwk::point_2d(head_pos.yaw, head_pos.pitch))
                           .magnitude();
        }
        if (dist < min_dist) {
            min_dist = dist;
            min_w = w;
        }
    }
    return min_w;
}

HeadControl::HeadControl(WorldModel* world_model) : world_model(world_model) {
    SensorData::instance().registerCallback(
            AsyncCallback<int64_t>("HeadControl", std::bind(&HeadControl::proceed, this, std::placeholders::_1)));

    auto* options = new OptionSet("HeadControl");
    options->addOption(new NaoControl::FloatOption("pitch_base_value", &pitch_base_value, 0.f, 1.f, .01f));
    NaoControl::RobotOption::instance().addOptionSet(options);
}

void HeadControl::proceed(int64_t time) {
    LolaComm::setHeadStiffness(1.f);
    if (!may_move.load()) {
        LolaComm::setHeadAngles(YawPitch{0, 0});
        return;
    }
    Focus cur_focus = focus.load();
    if (cur_focus == Focus::NOTHING)
        cur_focus = world_model->getOwnRobot().loc_quality > 0.7f ? Focus::BALL : Focus::LOC;
    time_step += (time - last_time) / 1'000'000.f;
    if (cur_focus == Focus::LOC) {
        sta_settings sta_yaw{1, 0.1f, 1.68f};
        if (cur_focus != last_focus)
            time_step = smoothTriAngStartTime(head_pos, sta_yaw);
        head_pos = {smoothTriAng(time_step, sta_yaw), .2f};
    } else if (cur_focus == Focus::BALL || cur_focus == Focus::BALL_GOALIE) {
        std::lock_guard<std::mutex> lck(ball_detection_mtx);
        auto striker = world_model->getStriker();
        bool striker_ball = striker && striker->ball;
        if (last_ball_percept > time - 2_s) {
            ball_found = true;
            float cur_ball_yaw = normalizeRotation(
                    ball_pos.yaw - get<Position>(SensorData::instance().getOdometry(last_ball_percept, time)).a);
            float target_yaw = clamped_linear_interpolation(abs(cur_ball_yaw), 0.f, cur_ball_yaw, 10_deg, 20_deg);
            float target_pitch = clamped_linear_interpolation(pitch_base_value - ball_pos.pitch, pitch_base_value,
                                                              ball_pos.pitch, 10_deg, 20_deg);
            head_pos.yaw = normalizeRotation(head_pos.yaw + normalizeRotation(target_yaw - head_pos.yaw) * 0.20f);
            // TODO: is 0.25 a good idea? Might be way too low, or otherwise 0 in ball search might be too high.
            head_pos.pitch = clamp(head_pos.pitch + (target_pitch - head_pos.pitch) * 0.15f, 0.25f, 0.5f);
        } else if (cur_focus == Focus::BALL_GOALIE && striker_ball) {
            ball_found = true;
            point_2d rel_team_ball = LocalizationUtils::absToRel(
                    LocalizationUtils::relToAbs(striker->ball->pos, striker->pos), world_model->getOwnRobot().pos);
            // estimate yaw and pitch, this could be done with a relToCam if we have it.
            head_pos.yaw = normalizeRotation(head_pos.yaw +
                                             normalizeRotation(rel_team_ball.to_direction() - head_pos.yaw) * 0.20f);
            float target_pitch = tan(pitch_base_value / (rel_team_ball.magnitude() + 0.001f));
            head_pos.pitch = clamp(head_pos.pitch + (target_pitch - head_pos.pitch) * 0.15f, 0.25f, 0.5f);
        } else {
            sta_settings sta_yaw{1, 0.1f, 1.68f};
            if (cur_focus != last_focus || ball_found) {
                time_step = smoothTriAngStartTime(head_pos, sta_yaw);
                ball_found = false;
            }
            // TODO: use last seen to determine direction of this.
            head_pos.yaw = smoothTriAng(time_step, sta_yaw);
            // Keep the last pitch we had when we saw the ball.
        }
    } else if (cur_focus == Focus::OBSTACLES) {
        sta_settings sta_yaw{2, 0.1f, 0.5f};
        if (cur_focus != last_focus)
            time_step = smoothTriAngStartTime(head_pos, sta_yaw);
        head_pos = {smoothTriAng(time_step, sta_yaw), .2f};
    } else if (cur_focus == Focus::BALL_SEARCH_LEFT) {
        sta_settings sta_yaw{4, 0.1f, 0.25f};
        sta_settings sta_pitch{4, 0.1f, 0.25f};
        if (cur_focus != last_focus)
            time_step = smoothTriAngStartTime(YawPitch(head_pos.yaw - .4f, head_pos.pitch - .25f), sta_yaw, sta_pitch);
        head_pos = {.4f + smoothTriAng(time_step, sta_yaw), smoothTriAng(time_step, sta_pitch) + 0.25f};
    } else if (cur_focus == Focus::BALL_SEARCH_RIGHT) {
        sta_settings sta_yaw{-4, 0.1f, 0.25f};
        sta_settings sta_pitch{4, 0.1f, 0.25f};
        if (cur_focus != last_focus)
            time_step = smoothTriAngStartTime(YawPitch(head_pos.yaw + .4f, head_pos.pitch - .25f), sta_yaw, sta_pitch);
        head_pos = {-.4f + smoothTriAng(time_step, sta_yaw), smoothTriAng(time_step, sta_pitch) + 0.25f};

    } else if (cur_focus == Focus::KEEP_STILL) {
        head_pos = {0, 1};
    }

    else {
        head_pos = {0, 0};
    }
    last_focus = cur_focus;
    last_time = time;
    LolaComm::setHeadAngles(head_pos);
}
