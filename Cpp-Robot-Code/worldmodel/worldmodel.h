#pragma once

#include <RoboCupGameControlData.h>
#include <SPLStandardMessage.h>
#include <envinfo.h>
#include <gamecontrollerclient.h>
//#include <localization_utils.h>
//#include <obstacleaggregator.h>
//#include <teamballupdater.h>
#include <threadsafe_deque.h>
#include <tracked_object.h>
#include <worldmodel.pb.h>
//#include <worldmodelmetadata.h>

#include <map>
#include <optional>
#include <set>
#include <shared_mutex>
#include <tuple>
#include <vector>

struct NetworkRobot {
    // Make sure to check that recv_time is != 0 before using any of the other fields!
    int64_t recv_time = 0;
    bool penalized = true;
    //protobuf::worldmodel::TeamCommMsg::Type recv_type;
    Robot robot;
};

class WorldModel {
public:
    WorldModel(int own_idx);
    ~WorldModel() = default;
    WorldModel(WorldModel&&) = delete;
    WorldModel(const WorldModel&) = delete;
    WorldModel& operator=(const WorldModel&) = delete;
    WorldModel& operator=(WorldModel&&) = delete;

    /**
     * @brief getTeamBall If a teamball exists it will be returned or an empty optional.
     * @return The Teamball or an empty option.
     * @note It is not guaranteed that a teamball exists!
     */
    std::optional<TeamBall> getTeamBall() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        return std::nullopt;
    }

    /**
     * @brief Get the obstacles from the team.
     * @return The obstacles of all robots.
     */
    std::vector<ObstacleObservation> getObstacles() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return std::vector<ObstacleObservation>();
    }

    /**
     * @brief Return a map of robots where we received a packet recently and we asume they are not penalized.
     * The own robot is never not alive but can be penalized.
     * @note Don't asume that the own robot from getAliveRobots() is the same as getOwnRobot(). It could be replaced
     * during two calls.
     * @note If you need getAliveRobots() and getTrustedPlayerIds() use getRobots()to get a consistent view.
     */
    std::map<PlayerIdx, Robot> getAliveRobots() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        std::map<PlayerIdx, Robot> robots;
        return robots;
    }

    /**
     * @brief Contains only the player PlayerIdx from player where the ball is in a certain range around our own
     * relative ball. Of course we trust always ourself.
     * @note If you need getAliveRobots() and getTrustedPlayerIds() use getRobots() to get a consistent view.
     */
    std::set<PlayerIdx> getTrustedPlayerIds() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        std::set<PlayerIdx> trustedIds;
        return trustedIds;
    }

    /**
     * @return Returns a consistent view on [AliveRobots, TrustedPlayerIds]
     */
    std::tuple<std::map<PlayerIdx, Robot>, std::set<PlayerIdx>> getRobots() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        std::map<PlayerIdx, Robot> robots;
        std::set<PlayerIdx> trustedIds;
        return std::make_tuple(robots, trustedIds);
    }

    //! @return The robot the firmware runs on.
    Robot getOwnRobot() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return Robot();
    }

    //! This must only used by the cam loop never by someone else.
    void setOwnRobot(Robot robot) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    }

    void setGameControllerData(const RoboCupGameControlData& data, int team_id) {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    }

    /*const RoboCupGameControlData& getGameControllerData() {
        return gcData;
    }*/

    //void update(const protobuf::worldmodel::TeamCommMsg& team_comm);
    //void updateNetworkSelf(const protobuf::worldmodel::TeamCommMsg& team_comm);
    void forceNetworkRobotBall(PlayerIdx idx, htwk::point_2d ball) {}
    void initialNetworkUpdate() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
    };

    std::vector<NetworkRobot> getNetworkRobots() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return std::vector<NetworkRobot>();
    }

    std::optional<Robot> getStriker() {
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */

        return std::nullopt;
    }

    std::map<PlayerIdx, Robot> getAliveNetworkRobots() {
        std::map<PlayerIdx, Robot> alive;
        /* This implementation was not changed, created or used as part of the Shared Autonomy Challenge. Please substitute your own implementation. */
        return alive;
    }
};
