cmake_minimum_required(VERSION 3.20)
project(atomtex-spe-file CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS ON "Build project tests")

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

add_subdirectory(${PROJECT_SOURCE_DIR}/atomtex_spe_file)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif ()
