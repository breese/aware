# FindBonjour
#
# Find Bonjour
#
# ::
#   Bonjour_INCLUDE_DIR
#   Bonjour_LIBRARY
#   Bonjour_FOUND
#
#   Bonjour_LIBRARIES
#   Bonjour_INCLUDE_DIRS
#
# Defines Bonjour::Bonjour imported target

mark_as_advanced(Bonjour_FOUND Bonjour_VERSION Bonjour_INCLUDE_DIR Bonjour_LIBRARY)

find_path(Bonjour_INCLUDE_DIR
  NAMES dns_sd.h)

find_library(Bonjour_LIBRARY
  NAMES dns_sd
)

set(Bonjour_FOUND FALSE CACHE BOOL "" FORCE)
if (EXISTS ${Bonjour_INCLUDE_DIR}/dns_sd.h)
  file(STRINGS ${Bonjour_INCLUDE_DIR}/dns_sd.h HEADER_FILE NEWLINE_CONSUME)
  string(REGEX MATCH "#define _DNS_SD_H[^\n]*" VERSION_LINE ${HEADER_FILE})
  string(REGEX REPLACE
    "#define _DNS_SD_H[ \t]*([0-9][0-9][0-9])([0-9][0-9])([0-9][0-9]).*"
    "\\1.\\2.\\3"
    Bonjour_VERSION
    ${VERSION_LINE})
  if ("${Bonjour_VERSION}" STREQUAL "${VERSION_LINE}")
    set(Bonjour_VERSION 0)
  else()
    set(Bonjour_FOUND TRUE CACHE BOOL "" FORCE)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Bonjour
    FOUND_VAR Bonjour_FOUND
    REQUIRED_VARS Bonjour_LIBRARY Bonjour_INCLUDE_DIR
    VERSION_VAR Bonjour_VERSION
)

if(Bonjour_FOUND)
    set(Bonjour_LIBRARIES ${Bonjour_LIBRARY})
    set(Bonjour_INCLUDE_DIRS ${Bonjour_INCLUDE_DIR})
    set(Bonjour_DEFINITIONS "")

    if (NOT TARGET Bonjour::Bonjour)
      add_library(Bonjour::Bonjour SHARED IMPORTED GLOBAL)
      set_target_properties(Bonjour::Bonjour PROPERTIES
        IMPORTED_LOCATION "${Bonjour_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${Bonjour_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${Bonjour_INCLUDE_DIR}"
	)
    endif()
endif()
