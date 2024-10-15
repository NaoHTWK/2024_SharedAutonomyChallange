#include "motion.h"

#include <iostream>
#include <tuple>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <stl_ext.h>

using namespace std;
namespace fs = boost::filesystem;

static const vector<pair<JointCategory, size_t>> old_joint_mapping{
        {JointCategory::HEAD, HeadYaw},       {JointCategory::HEAD, HeadPitch},    {JointCategory::ARM, LShoulderPitch},
        {JointCategory::ARM, LShoulderRoll},  {JointCategory::ARM, LElbowYaw},     {JointCategory::ARM, LElbowRoll},
        {JointCategory::LEG, HipYawPitch},    {JointCategory::LEG, LHipRoll},      {JointCategory::LEG, LHipPitch},
        {JointCategory::LEG, LKneePitch},     {JointCategory::LEG, LAnklePitch},   {JointCategory::LEG, LAnkleRoll},
        {JointCategory::LEG, HipYawPitch},    {JointCategory::LEG, RHipRoll},      {JointCategory::LEG, RHipPitch},
        {JointCategory::LEG, RKneePitch},     {JointCategory::LEG, RAnklePitch},   {JointCategory::LEG, RAnkleRoll},
        {JointCategory::ARM, RShoulderPitch}, {JointCategory::ARM, RShoulderRoll}, {JointCategory::ARM, RElbowYaw},
        {JointCategory::ARM, RElbowRoll},
};
static const vector<pair<JointCategory, size_t>> motion_editor_joint_mapping{
        {JointCategory::HEAD, HeadYaw},      {JointCategory::HEAD, HeadPitch},  {JointCategory::ARM, LShoulderPitch},
        {JointCategory::ARM, LShoulderRoll}, {JointCategory::ARM, LElbowYaw},   {JointCategory::ARM, LElbowRoll},
        {JointCategory::LEG, HipYawPitch},   {JointCategory::LEG, LHipRoll},    {JointCategory::LEG, LHipPitch},
        {JointCategory::LEG, LKneePitch},    {JointCategory::LEG, LAnklePitch}, {JointCategory::LEG, LAnkleRoll},
        {JointCategory::LEG, RHipRoll},      {JointCategory::LEG, RHipPitch},   {JointCategory::LEG, RKneePitch},
        {JointCategory::LEG, RAnklePitch},   {JointCategory::LEG, RAnkleRoll},  {JointCategory::ARM, RShoulderPitch},
        {JointCategory::ARM, RShoulderRoll}, {JointCategory::ARM, RElbowYaw},   {JointCategory::ARM, RElbowRoll},
};

Motion::Motion(const std::string& filename) {
    std::string file_to_load = filename;

    if(boost::filesystem::exists("/home/nao/firmware/data/motions/" + filename)) {
        file_to_load = "/home/nao/firmware/data/motions/" + filename;
    } else if(boost::filesystem::exists("motions/" + filename)) {
        file_to_load = "motions/" + filename;
    } else if(boost::filesystem::exists(filename)) {
        file_to_load = filename;
    } else {
        std::cerr << "Motion " << filename << " not found!" << std::endl;
    }

    fs::ifstream f(file_to_load);
    string version;
    getline(f, version);
    if (version == "2") {
        loadV2(f);
        return;
    }
    // Ignore the header.
    for (int i = 0; i < 4; i++) {
        string line;
        getline(f, line);
    }
    vector<Joints> motion_100hz;
    for (string line; getline(f, line);) {
        vector<string> vals;
        boost::algorithm::split(vals, line, boost::is_any_of(","));
        Joints frame;
        zip(vals, old_joint_mapping, [&frame](const string& v, pair<JointCategory, size_t> j) {
            if (j.first == JointCategory::ARM)
                frame.arms[j.second].angle = stof(v);
            else if (j.first == JointCategory::LEG)
                frame.legs[j.second].angle = stof(v);
            else if (j.first == JointCategory::HEAD)
                frame.head[j.second].angle = stof(v);
        });
        frame.arms[LWristYaw].angle = -90_deg;
        frame.arms[LHand].angle = 0.f;
        frame.arms[RWristYaw].angle = 90_deg;
        frame.arms[RHand].angle = 0.f;
        set_stiffness(1, &frame.arms);
        set_stiffness(1, &frame.legs);
        set_stiffness(1, &frame.head);
        motion_100hz.push_back(frame);
    }
    // Old motions are at 100Hz (10ms/frame), V6 uses 83.33Hz (12ms/frame).
    for (float f = 0; f < motion_100hz.size(); f += 12.f / 10.f) {
        int lower = floor(f);
        int upper = ceil(f);
        if (lower == upper)
            motion.push_back(motion_100hz[lower]);
        Joints result;
        joint_interpolation(motion_100hz[lower], motion_100hz[upper], f - lower, &result);
        motion.push_back(result);
    }
}

void Motion::loadV2(boost::filesystem::ifstream& f) {
    string line;
    getline(f, line);
    int num_frames = std::stoi(line);
    motion.resize(num_frames);
    stabilize.resize(num_frames);
    for (int i = 0; i < num_frames; i++) {
        string line;
        getline(f, line);
        vector<string> vals;
        boost::algorithm::split(vals, line, boost::is_any_of(","));
        Joints& frame = motion[i];
        zip(vals, motion_editor_joint_mapping, [&frame](const string& v, pair<JointCategory, size_t> j) {
            if (j.first == JointCategory::ARM)
                frame.arms[j.second].angle = stof(v);
            else if (j.first == JointCategory::LEG)
                frame.legs[j.second].angle = stof(v);
            else if (j.first == JointCategory::HEAD)
                frame.head[j.second].angle = stof(v);
        });
        frame.arms[LWristYaw].angle = -90_deg;
        frame.arms[LHand].angle = 0.f;
        frame.arms[RWristYaw].angle = 90_deg;
        frame.arms[RHand].angle = 0.f;
    }
    for (int i = 0; i < num_frames; i++) {
        string line;
        getline(f, line);
        vector<string> vals;
        boost::algorithm::split(vals, line, boost::is_any_of(","));
        Joints& frame = motion[i];
        zip(vals, motion_editor_joint_mapping, [&frame](const string& v, pair<JointCategory, size_t> j) {
            if (j.first == JointCategory::ARM)
                frame.arms[j.second].stiffness = stof(v);
            else if (j.first == JointCategory::LEG)
                frame.legs[j.second].stiffness = stof(v);
            else if (j.first == JointCategory::HEAD)
                frame.head[j.second].stiffness = stof(v);
        });
        frame.arms[LWristYaw].stiffness = 1.f;
        frame.arms[LHand].stiffness = 1.f;
        frame.arms[RWristYaw].stiffness = 1.f;
        frame.arms[RHand].stiffness = 1.f;
    }
    for (int i = 0; i < num_frames; i++) {
        string line;
        getline(f, line);
        stabilize[i] = stof(line);
    }
}
