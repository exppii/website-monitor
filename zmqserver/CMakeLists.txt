#file(GLOB HEDER_FILES "./*.h" "${PROTO_HDRS_DIR}/*.h" )
#file(GLOB SOURCE_FILES "./*.cc" "${PROTO_HDRS_DIR}/*.cc")

set(LINK_LIBS zmq)
#add_executable(zmqserver ${SOURCE_FILES} ${HEDER_FILES})
add_executable(zmqserver server.cc)
# add_dependencies(taskserver generated_code)
target_link_libraries(zmqserver ${LINK_LIBS})
