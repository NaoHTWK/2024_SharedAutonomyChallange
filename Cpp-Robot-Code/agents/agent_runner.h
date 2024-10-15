#pragma once

#include <memory>
#include <shared_mutex>
#include <thread>
#include <tuple>
#include <vector>

#include <agent_base.h>
#include <async.h>
#include <gamecontrollerclient.h>
#include <motion_command.h>
//#include <multi_target_tracker.h>
//#include <near_obstacle_tracker.h>
#include <noorder.h>
#include <worldmodel.h>

class AgentRunner {
public:
    AgentRunner(std::string agent_selection, GameControllerClient* gc_client/*, NearObstacleTracker* near_obstacle_tracker, MTtracker* far_obstacles*/);

    std::tuple<std::string, MotionCommand> proceed(const std::shared_ptr<Order>& order, WorldModel* world_model);

    static std::list<std::string> valid_agent_selections() {
        return {"competition", "walktuner", "ballIntercept"};
    };

private:
    std::vector<std::shared_ptr<AgentBase>> agents;
    std::unique_ptr<ThreadPool> thread_pool;
};
