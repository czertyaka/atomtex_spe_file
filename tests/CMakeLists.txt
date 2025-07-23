add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unit-tests)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fuzzing-tests)
endif ()
