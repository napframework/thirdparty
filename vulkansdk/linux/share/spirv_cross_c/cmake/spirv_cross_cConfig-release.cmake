#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-c" for configuration "Release"
set_property(TARGET spirv-cross-c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(spirv-cross-c PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "spirv-cross-glsl;spirv-cross-hlsl;spirv-cross-msl;spirv-cross-cpp;spirv-cross-reflect"
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.141.2/x86_64/lib/libspirv-cross-c.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-c )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-c "/root/sdk-build/1.2.141.2/x86_64/lib/libspirv-cross-c.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
