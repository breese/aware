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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

###############################################################################
# Boost package
###############################################################################

# FIXME: Probably ok to use older version
find_package(Boost 1.49.0 COMPONENTS unit_test_framework system thread)
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "${Boost_ERROR_REASON}")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

###############################################################################
# ZeroConf package
###############################################################################

find_package(ZeroConf REQUIRED)
if (${ZEROCONF_AVAHI_FOUND})
  include_directories(BEFORE ${ZEROCONF_AVAHI_INCLUDE_DIRS})
  link_directories(${ZEROCONF_AVAHI_LIBDIR})
  set(AWARE_ZEROCONF_TARGET ${ZEROCONF_AVAHI_LIBRARIES})

elseif (${ZEROCONF_BONJOUR_FOUND})
  include_directories(BEFORE ${ZEROCONF_BONJOUR_INCLUDE_DIRS})
  link_directories(${ZEROCONF_BONJOUR_LIBDIR})
  set(AWARE_ZEROCONF_TARGET ${ZEROCONF_BONJOUR_LIBRARIES})

else()
    message(FATAL_ERROR "No ZeroConf module found")
endif()

###############################################################################
# Aware package
###############################################################################

set(AWARE_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${AWARE_BUILD_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${AWARE_BUILD_DIR}/bin)

include_directories(BEFORE include)
link_directories(${AWARE_BUILD_DIR}/lib)

add_subdirectory(src)
add_library(aware STATIC
  ${AWARE_SOURCE}
)
target_compile_options(aware PRIVATE -Wall -Wextra)
target_link_libraries(aware ${AWARE_ZEROCONF_TARGET} ${EXTRA_LIBS})

###############################################################################
# Examples
###############################################################################

if(${ZEROCONF_AVAHI_FOUND})
  add_subdirectory(example/avahi EXCLUDE_FROM_ALL)
  add_custom_target(example DEPENDS avahi-example)
elseif(${ZEROCONF_BONJOUR_FOUND})
  add_subdirectory(example/bonjour EXCLUDE_FROM_ALL)
  add_custom_target(example DEPENDS bonjour-example)
endif()
