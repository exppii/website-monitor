cmake_minimum_required(VERSION 3.2)
project(WebsiteMonitor)

# set version
set(ELONSERVER_VERSION "0.1.0")
#
# Debugging Options
#
SET (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#SET(CMAKE_INSTALL_PREFIX /usr/local)
SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/target)

#local cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

# Initialize CXXFLAGS.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pthread")
  message("compile support c++14, now set cxx flag with: -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")
  message("compile support c++11, now set cxx flag with: -std=c++11")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.
  Please use a different C++ compiler.")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG          "$ENV{CXXFLAGS} -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "$ENV{CXXFLAGS} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "$ENV{CXXFLAGS} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "$ENV{CXXFLAGS} -O2 -g")

if(MSVC)
  SET(GCC_COVERAGE_LINK_FLAGS "/SAFESEH:NO")
  SET(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
endif()
#
# Project Output Paths
#
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/archive )

# for multi-config builds (e.g. msvc xcode)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/plugins )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/archive )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


FIND_PACKAGE (Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
#LINK_DIRECTORIES(${Protobuf_LIBRARY_DIRS})

FIND_PACKAGE (gRPC REQUIRED)
INCLUDE_DIRECTORIES(${gRPC_INCLUDE_DIRS})
LINK_DIRECTORIES(${gRPC_LIBRARY_DIRS})

include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_BINARY_DIR})

set(PROTO_HDRS_DIR "${PROJECT_BINARY_DIR}/protos")

file(GLOB PROTO_FILES "protos/*.proto" )
file(MAKE_DIRECTORY "${PROTO_HDRS_DIR}")

# get grpc_cpp_plugin in path
EXECUTE_PROCESS(COMMAND which grpc_cpp_plugin OUTPUT_VARIABLE GRPC_EXE)
if("${GRPC_EXE}" STREQUAL "")
  message(FATAL_ERROR "can not find grpc_cpp_plugin!!!")
  exit()
endif()
string(REGEX REPLACE "[\r\n\t]+" "" GRPC_CPP_GEN ${GRPC_EXE})
set(GRPC_FLAS "--plugin=protoc-gen-grpc=${GRPC_CPP_GEN}")

FOREACH(FIL ${PROTO_FILES})
  IF(FIL MATCHES "(.*)_service.proto")
    EXECUTE_PROCESS(
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --grpc_out=${PROTO_HDRS_DIR} ${GRPC_FLAS} --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/protos ${FIL}
    )
  ENDIF()
  EXECUTE_PROCESS(
      COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out=${PROTO_HDRS_DIR} --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/protos ${FIL}
  )
ENDFOREACH()


# Copy all config files
FILE(GLOB CONFIG_FILES conf/*.*)
FOREACH(_filepath ${CONFIG_FILES})
  get_filename_component(var ${_filepath} NAME)
  configure_file(${_filepath}  conf/${var} COPYONLY)
ENDFOREACH()

add_subdirectory(taskserver)
#add_subdirectory(webclient)
add_subdirectory(node)


#gtest
enable_testing()
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(test)
