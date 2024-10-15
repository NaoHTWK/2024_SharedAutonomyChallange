project(Orders)

add_library(${PROJECT_NAME}
    collectdataorder.h
    keepgoalorder.h
    moveballgoalorder.h
    moveballorder.h
    noorder.h
    order.cpp
    order.h
    penaltyshootorder.h
    # receivepassorder.h
    # tutorialorder.h
    walktopositionorder.h
    shootorder.h
    head_focus.h
    # walkrelativeorder.h
)
target_link_libraries(${PROJECT_NAME} NaoMessages utils protobuf_utils SoccerField)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    LINKER_LANGUAGE CXX)
