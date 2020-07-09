#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-c-shared" for configuration "Release"
set_property(TARGET spirv-cross-c-shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(spirv-cross-c-shared PROPERTIES
  IMPORTED_LOCATION_RELEASE "/root/sdk-build/1.2.141.2/x86_64/lib/libspirv-cross-c-shared.so.0.33.0"
  IMPORTED_SONAME_RELEASE "libspirv-cross-c-shared.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-c-shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-c-shared "/root/sdk-build/1.2.141.2/x86_64/lib/libspirv-cross-c-shared.so.0.33.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
