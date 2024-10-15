project(CameraUtils CXX)

add_library(${PROJECT_NAME}
    cammsg.h
    # imagememoryprovider.cpp
    # imagememoryprovider.h
    # localization_utils.cpp
    localization_utils.h
)

target_link_libraries(${PROJECT_NAME}
    utils
    # easy_profiler
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        .
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
