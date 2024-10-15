project(TeamStrategy)

find_package(Boost COMPONENTS filesystem REQUIRED QUIET)

add_library(${PROJECT_NAME}
    # noop_strategy.h
    # penalty_strategy.h
    # robocup_strategy.cpp
    # robocup_strategy.h
    # striker_strategy.cpp
    # striker_strategy.h
    # stay_strategy.cpp
    # stay_strategy.h
    team_strategy.cpp
    team_strategy.h
    team_strategy_factory.cpp
    team_strategy_factory.h
    # tutorial_strategy.h
    # dbhc_strategy.h
    # dbhc_strategy.cpp
    # moveball_strategy.cpp
    # moveball_strategy.h
    shared_manual_strategy.cpp
    shared_manual_strategy.h
    shared_autonomy_strategy.cpp
    shared_autonomy_strategy.h

    # features/field_search.cpp
    # features/field_search.h
    features/position_area.h
    # features/position_util.cpp
    features/position_util.h
    # features/ready_positioning.cpp
    # features/ready_positioning.h
    # features/striker_util.cpp
    # features/striker_util.h

    # standard_situations/field_defender.cpp
    # standard_situations/field_defender.h
    # standard_situations/line_defender.cpp
    # standard_situations/line_defender.h
    # standard_situations/penalty_kick.cpp
    # standard_situations/penalty_kick.h
)

option(TEAMSTRATEGY_ENABLE_DEBUG "Enable debug feature for the teamstrategy." OFF)

if(TEAMSTRATEGY_ENABLE_DEBUG)
    add_definitions(-DTS_DEBUG=1)
endif()

target_link_libraries(${PROJECT_NAME}
    SoccerField utils AgentUtils CameraUtils FlightrecorderClient GameControllerClientV5 Orders SPL TeamCommunication
    WalkingEngine WorldModel OptionFramework Boost::filesystem)
target_include_directories(${PROJECT_NAME} PUBLIC . features standard_situations)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
