project(LolaComm)

find_package(Boost COMPONENTS system REQUIRED QUIET)

add_library(${PROJECT_NAME}
    lola_comm.cpp
    lola_comm.h
)

target_link_libraries(${PROJECT_NAME}
    Motion
    SignalHandling
    utils
    pthread
    rt
    Boost::system
)

target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
