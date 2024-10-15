project(fw_sydney)

find_package(Boost REQUIRED COMPONENTS system program_options QUIET)

add_executable(${PROJECT_NAME}
#    fw_sydney.h
#    fw_sydney.cpp
#    gamecontroller_whistle_postprocessor.h
#    gamecontroller_whistle_postprocessor.cpp
    main.cpp
#    gamecontroller_visual_referee_postprocessor.h
#    gamecontroller_comm_postprocessor.h
)
target_link_libraries(${PROJECT_NAME}
    utils
    #pthread
    #Agents
    #Camera
    #CameraUtils
    #camera_stream
    #DaemonHandling
    #HeadControl
    #GameControllerClientV5
    #JpegCompressor
    #Localization
    #LolaComm
    #MotionControl
    #MultiTargetTracker
    #NearObstacleTracker
    OptionFramework
    #SensorData
    #SignalHandling
    #SoundProcessing
    #TeamCommunication
    #TeamStrategy
    #Vision
    #VisionLogger
    #Visualizer
    #WorldModel
    Boost::system
    Boost::program_options
    Boost::filesystem
    #clang_ubsan
)

target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
