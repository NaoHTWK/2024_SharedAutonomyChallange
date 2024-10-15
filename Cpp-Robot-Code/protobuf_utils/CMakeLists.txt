project(protobuf_utils)

add_library(${PROJECT_NAME}
    point_2d_helper.h
)

target_link_libraries(${PROJECT_NAME} utils NaoMessages)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    LINKER_LANGUAGE CXX)
