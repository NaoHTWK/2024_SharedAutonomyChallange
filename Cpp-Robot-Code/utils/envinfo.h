#pragma once

#include <point_2d.h>
#include <position.h>
#include <tracked_object.h>
#include <worldmodel.pb.h>

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#define WM_NUM_PLAYERS 7

using PlayerIdx = int;

struct RelativeBall {
    htwk::point_2d pos;            //!< relative position of the ball, best approximation from available measurements
    htwk::point_2d speed;          //!< absolute vector in meters per second
    htwk::point_2d highRiskSpeed;  //!< absolute vector in m/s, only useful in penalty shootouts
    htwk::point_2d mediumRiskSpeed; //!< absolute vector in m/s, useful when using ball speed in motion
    int64_t lastSeenTime = 0;      //!< us timestamps
};

struct RelativeHypo {
    htwk::point_2d pos{0, 0};  //!< relative position of the ball, best approximation from available measurements
    int64_t lastSeenTime = 0;  // us timestamps
};

struct TeamBall {
    htwk::point_2d pos;  //!< absolute position of the ball, best guess
    // htwk::point_2d speed;
    // float quality;  //!< [0 .. 1]
};

/// \brief The ObstacleObservation struct holds data relevant to the shared obstacle mechanism.
struct ObstacleObservation {
    htwk::point_2d pos;                           //!< absolute position of the observation
    std::int64_t last_quality_update;             //!< timestamp of last quality modification
    std::map<PlayerIdx, std::int64_t> observers;  //!< set of robots that observed that obstacle and it's timestamp
    float quality;                                //!< The quality (or confidence) of the observation.
    float own_team_prob;  //!< Probability that this is one of our own robots. 0 = opponent, 0.5 = unknown, 1 = us
};

struct Robot {
    PlayerIdx idx{};
    std::optional<RelativeBall> ball;  //!< The best ball position known to the robot.
    // The last hypothesis for a ball we have seen. Don't use this if you don't know what you're doing. You likely want
    // to use just the ball.
    std::optional<RelativeHypo> last_ball_hypothesis;
    Position pos;  //!< absolute position of the robot, matched for best fit in world model
    float loc_quality;
    bool isFallen{};
    bool penalized{};
    std::vector<htwk::TrackedObject> obstacles;  //!< Obstacles the robot sees and trys to avoid
    int64_t delta_time_since_whistle_on_recv_us = INT64_MAX;
    int64_t recv_time;
    int64_t sent_time;
    int64_t last_img_time;
    protobuf::worldmodel::TeamCommMsg::Type last_packet_type;
};
