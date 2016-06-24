#file(GLOB HEDER_FILES "../task/*.h")
#file(GLOB SOURCE_FILES "../task/*.cc")
#

##taskserver test

set(LOCAL_CACHED_TEST_LINK_LIBS grpc grpc++_unsecure protobuf dl leveldb)

add_executable(task_local_cached_test
        taskserver_local_cached_test.cc
        "taskserver_test.cc"
        "../common/varint.cc"
        "../common/utils.cc"
        "../taskserver/local_cache_util.cc"
        "../taskserver/options.cc"
        "${PROTO_HDRS_DIR}/master_service.pb.cc"
        "${PROTO_HDRS_DIR}/task_content.pb.cc"
        )
target_link_libraries(task_local_cached_test ${LOCAL_CACHED_TEST_LINK_LIBS}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a)

add_dependencies(task_local_cached_test googletest)

add_test(taskserver_test ${PROJECT_BINARY_DIR}/bin/task_local_cached_test)
#set(LINK_LIBS leveldb)
#
#add_executable(db_client_test levedb_client_test.cc
#        "../../common/varint.cc")
#target_link_libraries(db_client_test ${LINK_LIBS})
#
# node client test
set(NODE_LINK_LIBS grpc grpc++_unsecure protobuf dl curl leveldb)

file(GLOB NODE_SOURCE_FILES "../node/*.cc"
        "${PROTO_HDRS_DIR}/*.cc"
        "../node/task/*.cc"
        "../node/curl/*.cc"
        "../node/dataproc/*.cc"
        "../node/response/*.cc")

LIST(APPEND NODE_SOURCE_FILES
        "../common/varint.cc"
        "../common/utils.cc"
        "node_client_test.cc"
        "${PROTO_HDRS_DIR}/master_service.pb.cc"
        "${PROTO_HDRS_DIR}/task_content.pb.cc"
        #"node_client_task_test.cc"
        "node_client_local_cache_test.cc"
        "node_client_test.cc"
        )

list(REMOVE_ITEM NODE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/../node/node.cc")
add_executable(node_client_test ${NODE_SOURCE_FILES})

target_link_libraries(node_client_test ${NODE_LINK_LIBS}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a)

add_dependencies(node_client_test googletest)

add_test(node_client_test ${PROJECT_BINARY_DIR}/bin/node_client_test)
