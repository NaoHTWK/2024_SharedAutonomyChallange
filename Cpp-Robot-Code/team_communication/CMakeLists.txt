project(TeamCommunication)

find_package(Boost COMPONENTS filesystem REQUIRED QUIET)

add_library(${PROJECT_NAME}
    team_comm.cpp
    team_comm.h
)

option(TEAMSTRATEGY_ENABLE_DEBUG "Enable debug feature for the teamstrategy." OFF)

if(TEAMSTRATEGY_ENABLE_DEBUG)
    add_definitions(-DTS_DEBUG=1)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} WorldModel NaoMessages Boost::filesystem SPL utils SocketHelper)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
