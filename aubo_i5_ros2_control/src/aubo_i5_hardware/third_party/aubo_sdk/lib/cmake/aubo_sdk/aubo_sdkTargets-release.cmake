#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "aubo_sdk::aubo_sdk" for configuration "Release"
set_property(TARGET aubo_sdk::aubo_sdk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(aubo_sdk::aubo_sdk PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libaubo_sdk.so"
  IMPORTED_SONAME_RELEASE "libaubo_sdk.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS aubo_sdk::aubo_sdk )
list(APPEND _IMPORT_CHECK_FILES_FOR_aubo_sdk::aubo_sdk "${_IMPORT_PREFIX}/lib/libaubo_sdk.so" )

# Import target "aubo_sdk::robot_proxy" for configuration "Release"
set_property(TARGET aubo_sdk::robot_proxy APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(aubo_sdk::robot_proxy PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/librobot_proxy.so"
  IMPORTED_SONAME_RELEASE "librobot_proxy.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS aubo_sdk::robot_proxy )
list(APPEND _IMPORT_CHECK_FILES_FOR_aubo_sdk::robot_proxy "${_IMPORT_PREFIX}/lib/librobot_proxy.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
