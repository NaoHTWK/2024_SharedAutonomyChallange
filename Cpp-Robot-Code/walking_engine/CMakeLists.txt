project(WalkingEngine)

add_library(${PROJECT_NAME}
    ankle_balancer.cpp
    ankle_balancer.h
    # kalman_filter.cpp
    # kalman_filter.h
    # arm_controller.cpp
    # arm_controller.h
    # odometry.h
    # sit_motion.cpp
    # sit_motion.h
    # walking_engine.cpp
    # walking_engine.h
    # walking_time.h
)
target_link_libraries(${PROJECT_NAME} utils Kinematics IMU OptionFramework)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
)
