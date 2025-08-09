find_package(GTest REQUIRED)

set(ATOMTEX_SPE_FILE_SRC_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/src)
set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)

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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/example.spe
    ${CMAKE_CURRENT_BINARY_DIR}/example.spe
    COPYONLY
)

add_integration_test(spe_file_test
    spe_file_test.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/spe_file.cpp
    ${ATOMTEX_SPE_FILE_SRC_DIR}/measurement.cpp
)
