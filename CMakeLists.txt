###############################################################################
#
# http://github.com/breese/aware
#
# Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

# Debug build
#   cmake -DCMAKE_BUILD_TYPE=Debug .

###############################################################################

cmake_minimum_required(VERSION 2.8)
project(aware)

add_definitions(-g)
#add_definitions(-fmax-errors=1) # gcc
#add_definitions(-ferror-limit=1) # clang

###############################################################################
# Boost package
###############################################################################

# FIXME: Probably ok to use older version
find_package(Boost 1.49.0 COMPONENTS unit_test_framework system thread)
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "Boost not found (or too old)")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

###############################################################################
# Avahi package
###############################################################################

if (false)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVAHI avahi-client)
if (NOT ${AVAHI_FOUND})
  message(FATAL_ERROR "Please install the avahi-client package")
endif()
include_directories(${AVAHI_INCLUDE_DIRS})
# This is a workaround to get path of each library
foreach(lib ${AVAHI_LIBRARIES})
  find_library(${lib}_LIBRARY NAMES ${lib} PATHS ${AVAHI_LIBRARY_DIRS})
  list(APPEND AVAHI_LIBS ${${lib}_LIBRARY})
endforeach()
link_directories(${AVAHI_LIBRARY_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${AVAHI_LIBS})
else()
  set(AVAHI_BASE_DIR /home/breese/src/aware/avahi-0.6.31)
  set(AVAHI_INCLUDE_DIRS ${AVAHI_BASE_DIR})
  set(AVAHI_LIBRARY_DIRS ${AVAHI_BASE_DIR}/avahi-common/.libs ${AVAHI_BASE_DIR}/avahi-client/.libs)
  set(AVAHI_LIBS avahi-common avahi-client)
  include_directories(${AVAHI_INCLUDE_DIRS})
  link_directories(${AVAHI_LIBRARY_DIRS})
  set(EXTRA_LIBS ${EXTRA_LIBS} ${AVAHI_LIBS})
endif()

###############################################################################
# Aware package
###############################################################################

set(AWARE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories(BEFORE include)
link_directories(${AWARE_LIB_DIR})
set(LIBRARY_OUTPUT_PATH lib)

add_library(aware STATIC
  src/announce_socket.cpp
  src/contact.cpp
  src/monitor_socket.cpp
  src/detail/native_socket.cpp
  src/detail/avahi/announcer.cpp
  src/detail/avahi/browser.cpp
  src/detail/avahi/client.cpp
  src/detail/avahi/error.cpp
  src/detail/avahi/poller.cpp
)

###############################################################################
# Examples
###############################################################################

add_executable(zannounce
  example/zannounce.cpp
)
set_target_properties(zannounce PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
target_link_libraries(zannounce aware ${EXTRA_LIBS})

add_executable(zmonitor
  example/zmonitor.cpp
)
set_target_properties(zmonitor PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
target_link_libraries(zmonitor aware ${EXTRA_LIBS})
