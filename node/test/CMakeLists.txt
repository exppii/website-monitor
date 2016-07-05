file(GLOB HEDER_FILES "../task/*.h")
file(GLOB SOURCE_FILES "../task/*.cc")

#list(APPEND ${HEDER_FILES} "${PROTO_HDRS_DIR}/master_service.pb.h")
#list(APPEND ${SOURCE_FILES} "${PROTO_HDRS_DIR}/master_service.pb.cc")

#set(LINK_LIBS protobuf dl curl)
#add_executable(task_test ${SOURCE_FILES}
#        ${HEDER_FILES}
#        test_task.cc
#        "../response/curl_response.cc"
#        "../curl/curl_lib.cc"
#        "../curl/curl_header.cc"
#        "${PROTO_HDRS_DIR}/master_service.pb.cc"
#        "${PROTO_HDRS_DIR}/task_content.pb.cc"
#        )
#target_link_libraries(task_test ${LINK_LIBS})


#curl test

configure_file(../data/url_data.txt ${PROJECT_BINARY_DIR}/data/url_data.txt COPYONLY)
add_executable(curl_test
        curl_test.cc
        "../curl/curl_lib.cc"
        "../../common/utils.cc"
        "../curl/curl_header.cc"
        "../response/curl_response.cc"
        )
target_link_libraries(curl_test curl)


add_executable(slice_test slice_test.cc)
target_link_libraries(slice_test leveldb)

#task test

add_executable(ping_task_test ping_task_test.cc
        ../response/ping_response.cc
        ../util/ping_util.cc)
target_link_libraries(ping_task_test )