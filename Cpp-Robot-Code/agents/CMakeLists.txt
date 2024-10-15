project(Agents)

add_library(${PROJECT_NAME}
    agent_runner.cpp
    agent_runner.h
    agent_base.h
    # alignment_matrix.h
    collect_data.cpp
    collect_data.h
        # dribbling_matrix.h
    # goalie_dive.cpp
    # goalie_dive.h
    # goalie_nodive_positioning.cpp
    # goalie_nodive_positioning.h
    # localize.cpp
    # localize.h
    motion_command.cpp
    motion_command.h
    # search_ball.cpp
    # search_ball.h
    shoot.cpp
    shoot.h
    # tutorial.cpp
    # tutorial.h
    # walk_to_pos.cpp
    # walk_to_pos.h
    walk_tuner.cpp
    walk_tuner.h
    #     receive_pass.cpp receive_pass.h
    #     walk_relative.h walk_relative.cpp
)

target_link_libraries(${PROJECT_NAME} AgentUtils utils CameraUtils GameControllerClientV5 MultiTargetTracker NearObstacleTracker Orders SensorData SoccerField TeamStrategy WorldModel)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
