#pragma once

#include <cstdint>
#include <ctime>
#include <memory>

#include <cam_constants.h>
#include <imu.h>
//#include <localization_utils.h>
#include <point_3d.h>

struct cam_msg_t {
    uint32_t frame = 0;
    std::shared_ptr<uint8_t> img;
    //CamPose cam_pose;
    YPR body;
    YPR headJoint;
    CamID id;
    int64_t time = 0;  //!< in microseconds
};
