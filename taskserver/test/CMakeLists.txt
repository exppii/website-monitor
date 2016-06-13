#file(GLOB HEDER_FILES "../task/*.h")
#file(GLOB SOURCE_FILES "../task/*.cc")

set(LINK_LIBS leveldb)

add_executable(db_client_test levedb_client_test.cc
        "../../common/varint.cc")
target_link_libraries(db_client_test ${LINK_LIBS})
