project(GameControllerClientV5)

# For ASIO we need also system. ASIO is header only ... so we get ASIO for free ;-)
find_package(Boost REQUIRED COMPONENTS system QUIET)

add_library(${PROJECT_NAME}
    gamecontrollerclient.h
    gamecontrollerclient.cpp
    # gamecontrollerclientbutton.h
    # gamecontrollerclientbutton.cpp
    # gamecontrollerclientheartbeat.h
    # gamecontrollerclientheartbeat.cpp
    # gamecontrollerclientnetwork.h
    # gamecontrollerclientnetwork.cpp
)

target_link_libraries(${PROJECT_NAME} FlightrecorderClient LolaComm SignalHandling SPL utils Boost::boost Boost::system OptionFramework)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
