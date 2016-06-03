file(GLOB HEDER_FILES "../task/*.h")
file(GLOB SOURCE_FILES "../task/*.cc")

#list(APPEND ${HEDER_FILES} "${PROTO_HDRS_DIR}/master_service.pb.h")
#list(APPEND ${SOURCE_FILES} "${PROTO_HDRS_DIR}/master_service.pb.cc")

set(LINK_LIBS protobuf dl)
add_executable(task_test ${SOURCE_FILES}
        ${HEDER_FILES}
        test_task.cc
        "../response/curl_response.cc"
        "../curl/curl_lib.cc"
        "../curl/curl_header.cc"
        "${PROTO_HDRS_DIR}/master_service.pb.cc"
        "${PROTO_HDRS_DIR}/task_content.pb.cc"
        )
target_link_libraries(task_test ${LINK_LIBS})


#curl test


add_executable(curl_test
        curl_test.cc
        "../curl/curl_lib.cc"
        "../curl/curl_header.cc"
        )
target_link_libraries(task_test curl)