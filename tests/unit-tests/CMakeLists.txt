find_package(GTest REQUIRED)

add_custom_target(unit-tests)

set(ATOMTEX_SPE_FILE_SRC_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/src)
set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)

function(add_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_include_directories(${TEST_NAME} PRIVATE ${ATOMTEX_SPE_FILE_INCLUDE_DIR})
    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main GTest::gmock_main)
    target_compile_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    target_link_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    add_test(unit-test.${TEST_NAME} ${TEST_NAME})
    add_dependencies(unit-tests ${TEST_NAME})
endfunction()

add_unit_test(measurement_test
    ${CMAKE_CURRENT_SOURCE_DIR}/measurement_test.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/measurement.cpp
)

