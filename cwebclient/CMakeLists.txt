file(GLOB HEDER_FILES "./*.h" "${PROTO_HDRS_DIR}/*.h" )
file(GLOB SOURCE_FILES "./*.cc" "${PROTO_HDRS_DIR}/*.cc")

set(LINK_LIBS grpc grpc++_unsecure protobuf dl)
add_executable(webclient ${SOURCE_FILES} ${HEDER_FILES})
# add_dependencies(taskserver generated_code)
target_link_libraries(webclient ${LINK_LIBS})