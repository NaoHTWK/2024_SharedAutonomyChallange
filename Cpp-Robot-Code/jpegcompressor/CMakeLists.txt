cmake_minimum_required(VERSION 3.12)
project(JpegCompressor)

# For ASIO we need also system. ASIO is header only ... so we get ASIO for free ;-)
find_package(Boost REQUIRED COMPONENTS system QUIET)
find_package(JPEG REQUIRED)

add_library(${PROJECT_NAME}
    jpegcompressor.cpp
    jpegcompressor.h
    jpegcompressorconfig.h
    jpegcamimageprocessor.cpp
    jpegcamimageprocessor.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${JPEG_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    CameraUtils
    # FlightrecorderClient
    utils
    JPEG::JPEG
    Boost::boost Boost::system
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        .
)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
