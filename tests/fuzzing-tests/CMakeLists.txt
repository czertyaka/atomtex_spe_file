add_custom_target(fuzzing-tests)

set(ATOMTEX_SPE_FILE_SRC_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/src)
set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)

function(add_fuzzing_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    add_test(
        NAME "fuzzing-test.${TEST_NAME}"
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME} -max_total_time=60 -max_len=1048576
    )
    target_include_directories(${TEST_NAME} PRIVATE
        ${ATOMTEX_SPE_FILE_INCLUDE_DIR}
        ${ATOMTEX_SPE_FILE_SRC_DIR}
    )
    target_compile_options(${TEST_NAME} PRIVATE -fsanitize=fuzzer,address,undefined)
    target_link_options(${TEST_NAME} PRIVATE -fsanitize=fuzzer,address,undefined)
    add_dependencies(fuzzing-tests ${TEST_NAME})
endfunction()

add_fuzzing_test(lines_fuzzing_test
    lines_fuzzing_test.cpp
)

add_fuzzing_test(spe_file_fuzzing_test
    spe_file_fuzzing_test.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/spe_file.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/measurement.cpp
)
