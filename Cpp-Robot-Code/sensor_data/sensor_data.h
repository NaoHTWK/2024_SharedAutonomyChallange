#pragma once

#include <unistd.h>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <tuple>

//#include <easy/profiler.h>

#include <async.h>
#include <battery.h>
#include <cam_constants.h>
#include <cammsg.h>
//#include <flightrecorderlog.h>
#include <imu.h>
#include <joints.h>
#include <position.h>

inline HeadJointAngles interpolate(const HeadJointAngles& from, const HeadJointAngles& to, float f) {
    HeadJointAngles result;
    angle_interpolation(from, to, f, &result);
    return result;
};

inline YPR interpolate(const YPR& from, const YPR& to, float f) {
    return {from.yaw * (1.f - f) + to.yaw * f, from.pitch * (1.f - f) + to.pitch * f,
            from.roll * (1.f - f) + to.roll * f};
};

inline Battery interpolate(const Battery& from, const Battery& to, float f) {
    return {from.current * (1.f - f) + to.current * f, from.temp * (1.f - f) + to.temp * f,
            from.status * (1.f - f) + to.status * f, from.charge * (1.f - f) + to.charge * f};
};

inline float interpolate(float from, float to, float f) {
    return from * (1.f - f) + to * f;
};

class SensorData {
public:
    static SensorData& instance();
    static SensorData& instanceNoLog();

    YPR getBodyAngles(int64_t time) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        return YPR();
    }
    HeadJointAngles getHead(int64_t time) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        return HeadJointAngles();
    }
    float getLegHeight(int64_t time) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        return 0.f;
    }
    bool isStanding(int64_t time) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return false;
    }
    float get_walk_phase() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return 0.f;
    }
    YPR getLatestBodyAngles() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return YPR();
    }
    HeadJointAngles getLatestHead() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return HeadJointAngles();
    }
    // Callbacks will be called whenever new sensor data is available.
    void registerCallback(AsyncCallback<int64_t> callback) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    }

    void unregisterCallback(const std::string& name) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    }

    Battery getBattery(int64_t time) {
        return Battery();
    }

    // Returns the odometry as well as the actual to_time used.
    std::tuple<Position, int64_t> getOdometry(int64_t from_time, int64_t to_time);

    void addImage(int64_t time, CamID cam_id, const cam_msg_t& img);
    // This may only be used by one caller for each cam_id.
    cam_msg_t getImage(CamID cam_id, bool low_latency = false);  // blocking

    void cameraStreamStarted(CamID cam_id) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    }
};
