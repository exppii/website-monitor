FIND_PACKAGE (CURL REQUIRED)
INCLUDE_DIRECTORIES(${curl_INCLUDE_DIRS})
LINK_DIRECTORIES(${curl_LIBRARY_DIRS})

file(GLOB HEDER_FILES "./*.h" "${PROTO_HDRS_DIR}/*.h" )
file(GLOB SOURCE_FILES "./*.cc" "${PROTO_HDRS_DIR}/*.cc")

set(LINK_LIBS grpc grpc++_unsecure protobuf dl)
add_executable(nodeclient ${SOURCE_FILES} ${HEDER_FILES})
target_link_libraries(nodeclient ${LINK_LIBS})


#build example for test
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(test)
endif()