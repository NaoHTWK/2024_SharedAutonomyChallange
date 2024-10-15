#pragma once

#include <optional>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#include <joints.h>
#include <keyframe_controller.h>
#include <leds.h>
#include <touch_sensors.h>

class LolaComm {
public:
    static void setHeadJoints(const HeadJoints& head);
    static void setHeadStiffness(float stiffness);
    static void setHeadAngles(const HeadJointAngles& head);
    static void setHeadAngles(const YawPitch& head);
    static void setWalkRequest(const WalkRequest& walk_request);
    static void setDirectJoints(const Joints& joints, bool stabilize = false);
    static void unsetDirectJoints();
    static void playMotion(const KeyframeMotionID id);
    static void setLeftFootColor(const RGB& color);
    static void setRightFootColor(const RGB& color);
    static void setLeftEyeColor(const std::optional<RGB> &color);
    static void setRightEyeColor(const std::optional<std::array<RGB, 8>> &color);
    static void setChestColor(const RGB& color);
    static void setSkullLedStatus(const Leds::Skull& status);
    static void setAllowedToGetup(bool allowed_to_getup);
    static TouchSensors getTouchSensors();
    static Joints getJoints();
    static void fakeChestButtonPress();
    static void forceSit(bool sit);
    static JointMaxTemperature getJointMaxTemperature();

    static bool isFallen();

    // Keeps LolaConnector alive as long as the program is running.
    static void keepAlive();

private:
    LolaComm();
    static LolaComm& instance();

    // HeadJoints head_request;
    // WalkRequest walk_request;
    // std::optional<KeyframeMotionID> keyframe_request;
    // Joints joint_sensors;
    // std::optional<Joints> direct_joints;
    // bool direct_ankle_balancer;
    // TouchSensors touch_sensors;
    // RGB left_foot_color;
    // RGB right_foot_color;
    // std::optional<RGB> left_eye_color;
    // std::optional<RGB> right_eye_color;
    // RGB chest_color;
    // Leds::Skull skull_led_status;
    // bool allowed_to_getup;
    // bool force_sit;
    // bool fallen;

    HeadJoints* head_request = nullptr;
    WalkRequest* walk_request = nullptr;
    std::optional<KeyframeMotionID>* keyframe_request = nullptr;
    Joints* joint_sensors = nullptr;
    JointMaxTemperature* joint_max_temp = nullptr;
    std::optional<Joints>* direct_joints = nullptr;
    bool* direct_ankle_balancer = nullptr;
    TouchSensors* touch_sensors = nullptr;
    RGB* left_foot_color = nullptr;
    RGB* right_foot_color = nullptr;
    std::optional<RGB>* left_eye_color = nullptr;
    std::optional<std::array<RGB, 8>>* right_eye_color = nullptr;
    RGB* chest_color = nullptr;
    Leds::Skull* skull_led_status = nullptr;
    bool* allowed_to_getup = nullptr;
    static int64_t const DONT_FAKE_CHESTBUTTON_PRESS = 0;
    bool* force_sit = nullptr;
    bool* fallen = nullptr;
    int64_t* watchdog = nullptr;
    int64_t timestamp_fake_chestbutton_press = DONT_FAKE_CHESTBUTTON_PRESS;
    boost::interprocess::managed_shared_memory managed_shm;
    boost::interprocess::interprocess_mutex* mtx{};
};
