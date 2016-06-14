#.rst:
# FindgRPC
# --------
#
# Find gRPC
#
# Find the native gRPC headers and libraries.
#
# ::
#
#   gRPC_INCLUDE_DIRS   - where to find json/josn.h, etc.
#   gRPC_LIBRARY_DIRS      - List of libraries when using json.
#   gRPC_FOUND          - True if json found.
#   gRPC_VERSION_STRING - the version of json found

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2012 Rolf Eike Beer <eike@sf-mail.de>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Look for the header file.
set(_gRPC_SEARCHES)

# Search gRPC_ROOT first if it is set.
if(gRPC_ROOT)
  set(_gRPC_SEARCH_ROOT PATHS ${gRPC_ROOT} NO_DEFAULT_PATH)
  list(APPEND _gRPC_SEARCHES _gRPC_SEARCH_ROOT)
endif()

# Normal search.
set(_gRPC_SEARCH_NORMAL
  PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\;InstallPath]"
        "$ENV{PROGRAMFILES}"
        /usr/local
  )
list(APPEND _gRPC_SEARCHES _gRPC_SEARCH_NORMAL)

#set(gRPC_NAMES gRPC)

# Try each search configuration.
foreach(search ${_gRPC_SEARCHES})
  find_path(gRPC_INCLUDE_DIR NAMES grpc++/grpc++.h        ${${search}} PATH_SUFFIXES include)
  find_library(gRPC_LIBRARY  NAMES grpc++           ${${search}} PATH_SUFFIXES lib)
endforeach()

mark_as_advanced(gRPC_LIBRARY gRPC_INCLUDE_DIR)

if(gRPC_INCLUDE_DIR AND EXISTS "${gRPC_INCLUDE_DIR}/version.h")
    file(STRINGS "${gRPC_INCLUDE_DIR}/gRPC/version.h" gRPC_H REGEX "^.*gRPC_VERSION.*\"")

    string(REGEX REPLACE "^.*gRPC_VERSION \"([0-9]+).*$" "\\1" gRPC_VERSION_MAJOR "${gRPC_H}")
    string(REGEX REPLACE "^.*gRPC_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" gRPC_VERSION_MINOR  "${gRPC_H}")
    string(REGEX REPLACE "^.*gRPC_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" gRPC_VERSION_PATCH "${gRPC_H}")
    set(gRPC_VERSION_STRING "${gRPC_VERSION_MAJOR}.${gRPC_VERSION_MINOR}.${gRPC_VERSION_PATCH}")

    # only append a TWEAK version if it exists:
    set(gRPC_VERSION_TWEAK "")
    if( "${gRPC_H}" MATCHES "^.*gRPC_VERSION_STRING \"[0-9]+\\.[0-9]+\\.[0-9]+\\.([0-9]+).*$")
        set(gRPC_VERSION_TWEAK "${CMAKE_MATCH_1}")
        set(gRPC_VERSION_STRING "${gRPC_VERSION_STRING}.${gRPC_VERSION_TWEAK}")
    endif()

    set(gRPC_MAJOR_VERSION "${gRPC_VERSION_MAJOR}")
    set(gRPC_MINOR_VERSION "${gRPC_VERSION_MINOR}")
    set(gRPC_PATCH_VERSION "${gRPC_VERSION_PATCH}")
endif()

# handle the QUIETLY and REQUIRED arguments and set gRPC_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gRPC
                                  REQUIRED_VARS gRPC_LIBRARY gRPC_INCLUDE_DIR
                                  VERSION_VAR gRPC_VERSION_STRING)

if(gRPC_FOUND)
  get_filename_component(gRPC_LIBRARY_DIRS ${gRPC_LIBRARY} DIRECTORY)
  set(gRPC_INCLUDE_DIRS ${gRPC_INCLUDE_DIR})
endif()
