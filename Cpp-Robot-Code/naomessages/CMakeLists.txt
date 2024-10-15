project(NaoMessages)

find_package(Protobuf REQUIRED)

set(FirmwareProtoFiles
    log.proto
    log_indexed.proto
    lola_lowlevel.proto
    motioncontrol.proto
    visionlogging.proto
    visualizer.proto
    worldmodel.proto
)

set(MotionEditorProtoFiles
    motion.proto
    motion_editor.proto
)

add_library(${PROJECT_NAME} ${FirmwareProtoFiles})
protobuf_generate(TARGET ${PROJECT_NAME} LANGUAGE cpp ${FirmwareProtoFiles})

if(Protobuf_VERSION VERSION_LESS 5.0.0)
    set(Protobuf_Is_Stupid)
    message(STATUS "DON'T use protobuf abseil hack!")
else()
    message(STATUS "USE protobuf abseil hack!")
    find_package(absl REQUIRED)
    set(Protobuf_Is_Stupid absl::log_internal_message absl::log_internal_check_op)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC protobuf::libprotobuf
    ${Protobuf_Is_Stupid})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

# if(GRPC_FOUND)
#     set(MOTION_LIBRARY "MotionEditorMessages")
#     grpc_generate_cpp(GRPCSources  GRPCHeaders ${CMAKE_CURRENT_BINARY_DIR} ${MotionEditorProtoFiles})
#     add_library(${MOTION_LIBRARY} ${MotionEditorProflightrecorder_clienttoSources};${GRPCSources} ${GRPCHeaders} ${MotionEditorProtoFiles})
#     protobuf_generate(TARGET ${MOTION_LIBRARY} LANGUAGE cpp ${MotionEditorProtoFiles})
#     target_link_libraries(${MOTION_LIBRARY} protobuf::libprotobuf grpc grpc++)
#     target_include_directories(${MOTION_LIBRARY} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
#     set_target_properties(${MOTION_LIBRARY} PROPERTIES
#         CXX_STANDARD 17
#     )
# endif()
