project(WorldModel)

# For ASIO we need also system. ASIO is header only ... so we get ASIO for free ;-)
find_package(Boost REQUIRED COMPONENTS system QUIET)

add_library(${PROJECT_NAME}
        # obstacleaggregator.h
        # obstacleaggregator.cpp
        # teamballupdater.h
        # teamballupdater.cpp
        worldmodel.cpp
        worldmodel.h
        # worldmodelmetadata.h
        # worldmodelsender.cpp
        # worldmodelsender.h
)

target_link_libraries(${PROJECT_NAME} CameraUtils FlightrecorderClient GameControllerClientV5 LolaComm Orders SoccerField SPL Visualizer utils Boost::boost Boost::system)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
)
