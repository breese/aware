###############################################################################
#
# http://github.com/breese/aware
#
# Copyright (C) 2016 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

# ZEROCONF_BONJOUR_BUILD_DIR

option(ZEROCONF_WITH_AVAHI "Check for Avahi" ON)

set(ZEROCONF_FOUND FALSE)
set(ZEROCONF_AVAHI_FOUND FALSE)
set(ZEROCONF_BONJOUR_FOUND FALSE)
set(ZEROCONF_ANDROID_FOUND FALSE)

if (APPLE)
  # Included in libSystem
  set(ZEROCONF_BONJOUR_FOUND TRUE)
  set(ZEROCONF_FOUND ${ZEROCONF_BONJOUR_FOUND})

elseif(WIN32)
  # FIXME

else() # Posix
  if (ZEROCONF_WITH_AVAHI)
    find_package(PkgConfig QUIET REQUIRED)
    pkg_check_modules(ZEROCONF_AVAHI QUIET avahi-client)
    set(ZEROCONF_FOUND ${ZEROCONF_AVAHI_FOUND})
  endif()

  if (NOT ZEROCONF_FOUND)
    if (ZEROCONF_BONJOUR_BUILD_DIR)
      set(Bonjour_INCLUDE_DIR "${ZEROCONF_BONJOUR_BUILD_DIR}/mDNSShared")
      set(Bonjour_LIBRARY "${ZEROCONF_BONJOUR_BUILD_DIR}/mDNSPosix/build/prod/libdns_sd.so")
    endif()
    find_package(Bonjour REQUIRED)
    set(ZEROCONF_FOUND ${Bonjour_FOUND})
    set(ZEROCONF_BONJOUR_FOUND ${Bonjour_FOUND})
    set(ZEROCONF_BONJOUR_INCLUDE_DIRS ${Bonjour_INCLUDE_DIR})
    set(ZEROCONF_BONJOUR_LIBRARIES ${Bonjour_LIBRARIES})
  endif()
endif()
