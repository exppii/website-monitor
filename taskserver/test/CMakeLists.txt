#file(GLOB HEDER_FILES "../task/*.h")
#file(GLOB SOURCE_FILES "../task/*.cc")

set(LINK_LIBS leveldb)

add_executable(db_client_test levedb_client_test.cc
        "../../common/varint.cc")
target_link_libraries(db_client_test ${LINK_LIBS})

#local_cached_test

set(LOCAL_CACHED_TEST_LINK_LIBS grpc grpc++_unsecure protobuf dl leveldb)
add_executable(local_cached_test
        local_cached_util_test.cc
        "../../common/varint.cc"
        "../../common/utils.cc"
        "../local_cache_util.cc"
        "../options.cc"
        "${PROTO_HDRS_DIR}/master_service.pb.cc"
        "${PROTO_HDRS_DIR}/task_content.pb.cc"
        )
target_link_libraries(local_cached_test ${LOCAL_CACHED_TEST_LINK_LIBS})
