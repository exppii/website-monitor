cmake_minimum_required(VERSION 2.8.8)
project(gtest_builder C CXX)
include(ExternalProject)


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


ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -Dgtest_force_shared_crt=ON
        -DBUILD_GTEST=ON
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
        )

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest PARENT_SCOPE)
