project(utils)

find_package(Boost COMPONENTS filesystem REQUIRED QUIET)

set(srcs
    algorithm_ext.h
    async.h
    battery.h
    cam_constants.h
    # cam_feet_pos.h
    # cam_offset.cpp
    cam_offset.h
    cam_pose.cpp
    cam_pose.h
    # color.h
    envinfo.h
    exponential_moving_average.h
    # fftw_allocator.h
    # fsr.h
    imu.h
    # joints.h
    leds.cpp
    leds.h
    line.h
    # lola_frame.cpp
    # lola_frame.h
    # point_2d.cpp
    point_2d.h
    point_3d.h
    position.h
    # raster.h
    # ring_buffer.h
    # sound_player.h
    stl_ext.h
    threadsafe_deque.h
    # timed_sensor_data.h
    touch_sensors.h
    tracked_object.h
    # tracked_object.cpp
    # tts.h
    # tts.cpp
    # vector_field.h
    # vector_field.cpp
    # watchdog.h
    # LegPositionPlain.h
    # PositionPlain.h
)
add_library(${PROJECT_NAME} ${srcs})
# target_link_libraries(${PROJECT_NAME} easy_profiler Boost::filesystem NaoMessages msgpack-cxx)
target_link_libraries(${PROJECT_NAME} Boost::filesystem NaoMessages)

target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
