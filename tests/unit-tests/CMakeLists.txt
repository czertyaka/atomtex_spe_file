find_package(GTest REQUIRED)

add_custom_target(unit-tests)

set(ATOMTEX_SPE_FILE_SRC_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/src)
set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)

function(add_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    gtest_discover_tests(${TEST_NAME} TEST_PREFIX "unit-test.")
    target_include_directories(${TEST_NAME} PRIVATE
        ${ATOMTEX_SPE_FILE_INCLUDE_DIR}
        ${ATOMTEX_SPE_FILE_SRC_DIR}
    )
    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main GTest::gmock_main)
    target_compile_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    target_link_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    add_dependencies(unit-tests ${TEST_NAME})
endfunction()

add_unit_test(measurement_test
    ${CMAKE_CURRENT_SOURCE_DIR}/measurement_test.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/measurement.cpp
)

add_unit_test(lines_test
    ${CMAKE_CURRENT_SOURCE_DIR}/lines_test.cpp
)

add_unit_test(utf16le_file_test
    ${CMAKE_CURRENT_SOURCE_DIR}/utf16le_file_test.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/utf16le_file.cpp
)
