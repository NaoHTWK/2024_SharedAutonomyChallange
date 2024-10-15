project(HeadControl)

add_library(${PROJECT_NAME}
    head_control.cpp
    head_control.h
)

target_link_libraries(${PROJECT_NAME} utils Agents LolaComm SensorData OptionFramework)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
