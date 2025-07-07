cmake_minimum_required(VERSION 3.20)
project(atomtex-spe-file CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS ON "Build project tests")

add_subdirectory(${PROJECT_SOURCE_DIR}/atomtex_spe_file)

if (BUILD_TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif ()
