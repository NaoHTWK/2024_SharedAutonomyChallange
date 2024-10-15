project(Motion)

find_package(Boost COMPONENTS filesystem system REQUIRED QUIET)

add_library(${PROJECT_NAME}
    # getup_controller.cpp
    # getup_controller.h
    keyframe_controller.cpp
    keyframe_controller.h
    motion.cpp
    motion.h
)
target_link_libraries(${PROJECT_NAME} utils Boost::system Boost::filesystem WalkingEngine)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
)
