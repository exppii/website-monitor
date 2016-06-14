file(GLOB HEDER_FILES "./*.h" "${PROTO_HDRS_DIR}/*.h" )
file(GLOB SOURCE_FILES "./*.cc" "${PROTO_HDRS_DIR}/*.cc")
#list(REMOVE_ITEM SOURCE_FILES "/Users/liuyou/Workspace/ElonServer/taskserver/./grpc_master_service.cc")

LIST(APPEND HEDER_FILES "../common/utils.h" "../common/varint.h")

LIST(APPEND SOURCE_FILES "../common/utils.cc" "../common/varint.cc")

set(LINK_LIBS grpc grpc++_unsecure protobuf dl leveldb)
add_executable(taskserver ${SOURCE_FILES} ${HEDER_FILES})
# add_dependencies(taskserver generated_code)
target_link_libraries(taskserver ${LINK_LIBS})
