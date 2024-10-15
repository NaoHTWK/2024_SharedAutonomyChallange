project(SPL)

add_library(${PROJECT_NAME}
        INTERFACE
        RoboCupGameControlData.h
        SPLStandardMessage.h

)

target_include_directories(${PROJECT_NAME} INTERFACE .)
