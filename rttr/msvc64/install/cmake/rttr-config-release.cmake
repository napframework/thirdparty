#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RTTR::Core" for configuration "Release"
set_property(TARGET RTTR::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RTTR::Core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/rttr_core.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/rttr_core.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTTR::Core )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTTR::Core "${_IMPORT_PREFIX}/lib/rttr_core.lib" "${_IMPORT_PREFIX}/bin/rttr_core.dll" )

# Import target "RTTR::Core_Lib" for configuration "Release"
set_property(TARGET RTTR::Core_Lib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RTTR::Core_Lib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX;RC"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/librttr_core.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTTR::Core_Lib )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTTR::Core_Lib "${_IMPORT_PREFIX}/lib/librttr_core.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
