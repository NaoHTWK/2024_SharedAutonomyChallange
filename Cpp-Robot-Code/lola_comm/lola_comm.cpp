#include "lola_comm.h"

#include <functional>
#include <mutex>

#include <signalhandling.h>
#include <stl_ext.h>

namespace bi = boost::interprocess;
using namespace std;

void LolaComm::setHeadJoints(const HeadJoints& head) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.head_request = head;
    *self.watchdog = time_us();
}

void LolaComm::setHeadAngles(const HeadJointAngles& head) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    set_angles(head, self.head_request);
    *self.watchdog = time_us();
}

void LolaComm::setHeadAngles(const YawPitch& head) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    set_angles({head.yaw, head.pitch}, self.head_request);
    *self.watchdog = time_us();
}

void LolaComm::setHeadStiffness(float stiffness) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    set_stiffness(stiffness, self.head_request);
    *self.watchdog = time_us();
}

void LolaComm::setWalkRequest(const WalkRequest& walk_request) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.walk_request = walk_request;
    *self.watchdog = time_us();
}

void LolaComm::setDirectJoints(const Joints& joints, bool stabilize) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.direct_joints = joints;
    *self.direct_ankle_balancer = stabilize;
    *self.watchdog = time_us();
}

void LolaComm::unsetDirectJoints() {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.direct_joints = nullopt;
    *self.watchdog = time_us();
}

void LolaComm::playMotion(KeyframeMotionID id) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.keyframe_request = id;
    *self.watchdog = time_us();
}

void LolaComm::setLeftFootColor(const RGB& color) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.left_foot_color = color;
}

void LolaComm::setRightFootColor(const RGB& color) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.right_foot_color = color;
}

void LolaComm::setLeftEyeColor(const std::optional<RGB>& color) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.left_eye_color = color;
}

void LolaComm::setRightEyeColor(const std::optional<std::array<RGB, 8>>& color) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.right_eye_color = color;
}

void LolaComm::setChestColor(const RGB& color) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.chest_color = color;
}

void LolaComm::setSkullLedStatus(const Leds::Skull& status) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.skull_led_status = status;
}

void LolaComm::fakeChestButtonPress() {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    self.timestamp_fake_chestbutton_press = time_us();
}

void LolaComm::forceSit(bool sit) {
    if (SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.force_sit = sit;
}

JointMaxTemperature LolaComm::getJointMaxTemperature()
{
    if (SignalHandling::shutdown())
        return {};

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    return *self.joint_max_temp;
}

TouchSensors LolaComm::getTouchSensors() {
    if(SignalHandling::shutdown())
        return {};

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    if (self.timestamp_fake_chestbutton_press != DONT_FAKE_CHESTBUTTON_PRESS && (time_us() - self.timestamp_fake_chestbutton_press) <= 1._s) {
        self.touch_sensors->chestButton = 1.0f;
    }
    return *self.touch_sensors;
}

Joints LolaComm::getJoints() {
    if(SignalHandling::shutdown())
        return {};

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    return *self.joint_sensors;
}

bool LolaComm::isFallen() {
    if(SignalHandling::shutdown())
        return false;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    return *self.fallen;
}

void LolaComm::setAllowedToGetup(bool allowed_to_getup) {
    if(SignalHandling::shutdown())
        return;

    LolaComm& self = instance();
    bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
    *self.allowed_to_getup = allowed_to_getup;
}

LolaComm::LolaComm() : managed_shm{bi::open_only, "LolaConnector"} {
    std::tie(head_request, std::ignore) = managed_shm.find<HeadJoints>("HeadRequest");
    std::tie(walk_request, std::ignore) = managed_shm.find<WalkRequest>("WalkRequest");
    std::tie(keyframe_request, std::ignore) = managed_shm.find<optional<KeyframeMotionID>>("KeyframeRequest");
    std::tie(touch_sensors, std::ignore) = managed_shm.find<TouchSensors>("TouchSensors");
    std::tie(joint_sensors, std::ignore) = managed_shm.find<Joints>("JointSensors");
    std::tie(direct_joints, std::ignore) = managed_shm.find<std::optional<Joints>>("DirectJoints");
    std::tie(direct_ankle_balancer, std::ignore) = managed_shm.find<bool>("DirectAnkleBalancer");
    std::tie(fallen, std::ignore) = managed_shm.find<bool>("Fallen");
    std::tie(watchdog, std::ignore) = managed_shm.find<int64_t>("Watchdog");
    std::tie(left_foot_color, std::ignore) = managed_shm.find<RGB>("FootLeftColor");
    std::tie(right_foot_color, std::ignore) = managed_shm.find<RGB>("FootRightColor");
    std::tie(left_eye_color, std::ignore) = managed_shm.find<std::optional<RGB>>("EyeLeftColor");
    std::tie(right_eye_color, std::ignore) = managed_shm.find<std::optional<std::array<RGB, 8>>>("EyeRightColor");
    std::tie(chest_color, std::ignore) = managed_shm.find<RGB>("ChestColor");
    std::tie(skull_led_status, std::ignore) = managed_shm.find<Leds::Skull>("SkullLedStatus");
    std::tie(allowed_to_getup, std::ignore) = managed_shm.find<bool>("AllowedToGetup");
    std::tie(force_sit, std::ignore) = managed_shm.find<bool>("ForceSit");
    std::tie(joint_max_temp, std::ignore) = managed_shm.find<JointMaxTemperature>("JointMaxTemperature");
    std::tie(mtx, std::ignore) = managed_shm.find<bi::interprocess_mutex>("LolaConnectorMutex");
}

LolaComm& LolaComm::instance() {
    static LolaComm instance;
    return instance;
}

void LolaComm::keepAlive() {
    launch_named_thread("LolaCommWdg", false, []() {
        LolaComm& self = instance();
        while (!SignalHandling::shutdown()) {
            {
                bi::scoped_lock<bi::interprocess_mutex> lock{*self.mtx};
                *self.watchdog = time_us();
            }
            usleep(0.5_s);
        }
    }).detach();
}
