find_package(GTest REQUIRED)

add_custom_target(integration-tests)

function(add_integration_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    add_test(
        NAME "integration-test.${TEST_NAME}"
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
    )
    target_include_directories(${TEST_NAME} PRIVATE
        ${ATOMTEX_SPE_FILE_INCLUDE_DIR}
    )
    target_link_libraries(${TEST_NAME} PRIVATE atomtex_spe_file GTest::gtest_main GTest::gmock_main)
    target_compile_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    target_link_options(${TEST_NAME} PRIVATE -fsanitize=address,undefined)
    add_dependencies(integration-tests ${TEST_NAME})
endfunction()
