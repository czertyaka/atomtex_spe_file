find_package(benchmark REQUIRED)

set(ATOMTEX_SPE_FILE_SRC_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/src)
set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/example.spe
    ${CMAKE_CURRENT_BINARY_DIR}/example.spe
    COPYONLY
)

add_executable(spe_file_benchmark
    spe_file_benchmark.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/spe_file.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/measurement.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/utf16le_file.cpp
)

target_include_directories(spe_file_benchmark PRIVATE
    ${ATOMTEX_SPE_FILE_INCLUDE_DIR}
    ${ATOMTEX_SPE_FILE_SRC_DIR}
)

target_link_libraries(spe_file_benchmark benchmark::benchmark_main)
