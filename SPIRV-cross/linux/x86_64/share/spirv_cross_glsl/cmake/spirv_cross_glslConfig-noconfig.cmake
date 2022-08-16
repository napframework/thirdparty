#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-glsl" for configuration ""
set_property(TARGET spirv-cross-glsl APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(spirv-cross-glsl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libspirv-cross-glsl.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-glsl )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-glsl "${_IMPORT_PREFIX}/lib/libspirv-cross-glsl.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
