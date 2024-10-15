project(SignalHandling)

find_package(Boost COMPONENTS filesystem REQUIRED QUIET)

add_library(${PROJECT_NAME}
    signalhandling.h
    signalhandling.cpp
)
target_link_libraries(${PROJECT_NAME} utils Boost::filesystem)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
