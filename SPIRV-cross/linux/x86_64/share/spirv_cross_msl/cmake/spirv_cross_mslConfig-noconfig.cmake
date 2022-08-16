#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-msl" for configuration ""
set_property(TARGET spirv-cross-msl APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(spirv-cross-msl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libspirv-cross-msl.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-msl )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-msl "${_IMPORT_PREFIX}/lib/libspirv-cross-msl.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
