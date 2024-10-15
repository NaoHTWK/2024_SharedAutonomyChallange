#include <agent_runner.h>
#include <algorithm_ext.h>
#include <collect_data.h>
//#include <dribble.h>
//#include <goalie_dive.h>
//#include <goalie_nodive_positioning.h>
#include <leds.h>
//#include <localize.h>
#include <lola_comm.h>
//#include <receive_pass.h>
//#include <search_ball.h>
#include <shoot.h>
#include <stl_ext.h>
//#include <tutorial.h>
//#include <walk_to_pos.h>
#include <walk_tuner.h>

#include <future>
//#include "walk_relative.h"

AgentRunner::AgentRunner(std::string agent_selection, GameControllerClient* gc_client/*,
                         NearObstacleTracker* near_obstacle_tracker, MTtracker* far_obstacles*/) {

    fprintf(stdout, "Start agent selection of: %s\n", agent_selection.c_str());
    fflush(stdout);

    // Agents registered first trump agents registered later.
    if (agent_selection == "competition") {
        // agents.emplace_back(new TutorialAgent);
        //agents.emplace_back(new CollectDataAgent);
        //agents.emplace_back(new GoalieDiveAgent);
        //agents.emplace_back(new GoalieNoDivePositioningAgent);
        //agents.emplace_back(new WalkRelativeAgent(near_obstacle_tracker));
        //agents.emplace_back(new LocalizeAgent(gc_client));
        //agents.emplace_back(new SearchBallAgent);
        agents.emplace_back(new ShootAgent);
        //agents.emplace_back(new ReceivePassAgent);
        //agents.emplace_back(new DribbleAgent(/*near_obstacle_tracker*/));
        //agents.emplace_back(new WalkToPosAgent(/*near_obstacle_tracker*/));
    } else if (agent_selection == "walktuner") {
        agents.emplace_back(new ShootAgent);
        //agents.emplace_back(new DribbleAgent(/*near_obstacle_tracker*/));
        //agents.emplace_back(new ReceivePassAgent);
        agents.emplace_back(new WalkTuner);
        //agents.emplace_back(new WalkToPosAgent(near_obstacle_tracker));
        //agents.emplace_back(new LocalizeAgent(gc_client));
        //agents.emplace_back(new WalkRelativeAgent(near_obstacle_tracker));
        //agents.emplace_back(new SearchBallAgent);


    } else if (agent_selection == "ballIntercept") {
        //agents.emplace_back(new LocalizeAgent(gc_client));
        //agents.emplace_back(new SearchBallAgent);
        //agents.emplace_back(new ReceivePassAgent);
        //agents.emplace_back(new WalkToPosAgent(near_obstacle_tracker));
    }

    thread_pool = std::make_unique<ThreadPool>("AgentPool", agents.size());
}

std::tuple<std::string, MotionCommand> AgentRunner::proceed(const std::shared_ptr<Order>& order,
                                                            WorldModel* world_model) {
    LolaComm::setLeftEyeColor({});
    LolaComm::setRightEyeColor({});
    std::vector<std::shared_ptr<std::packaged_task<std::tuple<std::string, MotionCommand>()>>> agent_futures;
    for (const auto& agent : agents) {
        auto ptr = thread_pool->run<std::tuple<std::string, MotionCommand>>([agent, order, world_model]() {
            return std::make_tuple(agent->name, agent->proceed(order, world_model));
        });
        agent_futures.push_back(ptr);
    }

    std::vector<std::tuple<std::string, MotionCommand>> agent_results;
    htwk::transform_insert(agent_futures, agent_results, [](auto f) { return f->get_future().get(); });

    // Use the first agent which returns something else then Nothing. -> Order of agents decides which agent is executed
    return htwk::find_with_default(
            agent_results,
            [](const std::tuple<std::string, MotionCommand>& mc) {
                return std::get<MotionCommand>(mc) != MotionCommand::Nothing;
            },
            std::make_tuple("No Agent", MotionCommand::Nothing));
}
