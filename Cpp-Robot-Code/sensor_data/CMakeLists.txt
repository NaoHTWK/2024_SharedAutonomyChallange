project(SensorData)

add_library(${PROJECT_NAME}
    #sensor_data.cpp
    sensor_data.h
)
target_link_libraries(${PROJECT_NAME}
    Boost::system
    easy_profiler
    rt
    utils
    pthread
    CameraUtils
    FlightrecorderClient
    NaoMessages
    SignalHandling
    OptionFramework
    utils
)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
)
