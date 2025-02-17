# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\SortSimple_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SortSimple_autogen.dir\\ParseCache.txt"
  "SortSimple_autogen"
  )
endif()
