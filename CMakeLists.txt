cmake_minimum_required(VERSION 3.20)
project(atomtex-spe-file VERSION
    "${ATOMTEX_SPE_FILE_VERSION_MAJOR}.${ATOMTEX_SPE_FILE_VERSION_MINOR}.${ATOMTEX_SPE_FILE_VERSION_PATCH}")
enable_language(CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS ON "Build project tests")

add_compile_definitions(
    ATOMTEX_SPE_FILE_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
    ATOMTEX_SPE_FILE_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
    ATOMTEX_SPE_FILE_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
    ATOMTEX_SPE_FILE_VERSION=${CMAKE_PROJECT_VERSION}
)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    # this is to suppress
    # clang++: warning: argument unused during compilation: '-stdlib=libstdc++' [-Wunused-command-line-argument]
    # in Nix environment
    # https://github.com/conan-io/conan/issues/19006
    add_compile_options(-Wno-unused-command-line-argument)
endif ()

add_subdirectory(${PROJECT_SOURCE_DIR}/atomtex_spe_file)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif ()
