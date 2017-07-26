#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RTTR::Core" for configuration "Debug"
set_property(TARGET RTTR::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RTTR::Core PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/librttr_core_d.0.9.6.dylib"
  IMPORTED_SONAME_DEBUG "librttr_core_d.0.9.6.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTTR::Core )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTTR::Core "${_IMPORT_PREFIX}/bin/librttr_core_d.0.9.6.dylib" )

# Import target "RTTR::Core_Lib" for configuration "Debug"
set_property(TARGET RTTR::Core_Lib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RTTR::Core_Lib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/librttr_core_d.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTTR::Core_Lib )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTTR::Core_Lib "${_IMPORT_PREFIX}/lib/librttr_core_d.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
