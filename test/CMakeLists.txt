#file(GLOB HEDER_FILES "../task/*.h")
#file(GLOB SOURCE_FILES "../task/*.cc")
#

message(${GTEST_INCLUDE_DIRS})
##local_cached_test

set(LOCAL_CACHED_TEST_LINK_LIBS grpc grpc++_unsecure protobuf dl leveldb)

add_executable(task_local_cached_test
        taskserver_local_cached_test.cc
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

add_test(test1 ${PROJECT_BINARY_DIR}/bin/task_local_cached_test)
#set(LINK_LIBS leveldb)
#
#add_executable(db_client_test levedb_client_test.cc
#        "../../common/varint.cc")
#target_link_libraries(db_client_test ${LINK_LIBS})
#

#


