#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-util" for configuration ""
set_property(TARGET spirv-cross-util APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(spirv-cross-util PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "spirv-cross-core"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libspirv-cross-util.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-util )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-util "${_IMPORT_PREFIX}/lib/libspirv-cross-util.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
