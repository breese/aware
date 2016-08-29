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
      set(ZEROCONF_BONJOUR_INCLUDE_DIRS "${ZEROCONF_BONJOUR_BUILD_DIR}/mDNSShared")
      set(ZEROCONF_BONJOUR_LIBDIR "${ZEROCONF_BONJOUR_BUILD_DIR}/mDNSPosix/build/prod")
      set(ZEROCONF_BONJOUR_LIBRARIES "dns_sd")
      set(ZEROCONF_BONJOUR_FOUND TRUE)
    else()
      # FIXME: Not tested
      find_path(ZEROCONF_BONJOUR_INCLUDE_DIRS dns_sd.h)
      set(ZEROCONF_BONJOUR_LIBRARIES "dns_sd")
      find_library(ZEROCONF_BONJOUR_LIBDIR NAME ${ZEROCONF_BONJOUR_LIBRARIES})
      set(ZEROCONF_BONJOUR_FOUND TRUE)
    endif()
    set(ZEROCONF_FOUND ${ZEROCONF_BONJOUR_FOUND})
  endif()
endif()
