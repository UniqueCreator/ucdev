#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RocketCore" for configuration "Debug"
set_property(TARGET RocketCore APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RocketCore PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/RocketCore.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "D:/projects/freetype/freetype-2.7/objs/vc2010/x64/freetype27.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/RocketCore.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS RocketCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_RocketCore "${_IMPORT_PREFIX}/lib/RocketCore.lib" "${_IMPORT_PREFIX}/bin/RocketCore.dll" )

# Import target "RocketControls" for configuration "Debug"
set_property(TARGET RocketControls APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RocketControls PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/RocketControls.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "RocketCore"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/RocketControls.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS RocketControls )
list(APPEND _IMPORT_CHECK_FILES_FOR_RocketControls "${_IMPORT_PREFIX}/lib/RocketControls.lib" "${_IMPORT_PREFIX}/bin/RocketControls.dll" )

# Import target "RocketDebugger" for configuration "Debug"
set_property(TARGET RocketDebugger APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RocketDebugger PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/RocketDebugger.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "RocketCore"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/RocketDebugger.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS RocketDebugger )
list(APPEND _IMPORT_CHECK_FILES_FOR_RocketDebugger "${_IMPORT_PREFIX}/lib/RocketDebugger.lib" "${_IMPORT_PREFIX}/bin/RocketDebugger.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
