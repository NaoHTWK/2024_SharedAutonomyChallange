project(SoccerField)

find_package(Boost COMPONENTS filesystem REQUIRED QUIET)

add_library(${PROJECT_NAME}
    soccerfield.h
    soccerfield.cpp
    soccerfielddefinitions.h
    soccerfielddefinitions.cpp
    soccerfieldinstance.h
    soccerfieldinstance.cpp
)

target_link_libraries(${PROJECT_NAME} FlightrecorderClient SignalHandling utils nlohmann_json Boost::filesystem)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
)
