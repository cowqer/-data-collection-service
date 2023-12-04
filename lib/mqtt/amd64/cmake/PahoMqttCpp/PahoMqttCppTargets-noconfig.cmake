#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PahoMqttCpp::paho-mqttpp3" for configuration ""
set_property(TARGET PahoMqttCpp::paho-mqttpp3 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PahoMqttCpp::paho-mqttpp3 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.so.1.2.0"
  IMPORTED_SONAME_NOCONFIG "libpaho-mqttpp3.so.1"
  )

list(APPEND _cmake_import_check_targets PahoMqttCpp::paho-mqttpp3 )
list(APPEND _cmake_import_check_files_for_PahoMqttCpp::paho-mqttpp3 "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.so.1.2.0" )

# Import target "PahoMqttCpp::paho-mqttpp3-static" for configuration ""
set_property(TARGET PahoMqttCpp::paho-mqttpp3-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PahoMqttCpp::paho-mqttpp3-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.a"
  )

list(APPEND _cmake_import_check_targets PahoMqttCpp::paho-mqttpp3-static )
list(APPEND _cmake_import_check_files_for_PahoMqttCpp::paho-mqttpp3-static "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
