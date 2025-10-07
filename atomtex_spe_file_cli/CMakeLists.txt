find_package(Boost REQUIRED COMPONENTS program_options json)

add_executable(atomtex_spe_file_cli main.cpp run.cpp)
target_link_libraries(atomtex_spe_file_cli PRIVATE
  Boost::program_options
  Boost::json
  atomtex_spe_file
)

set(ATOMTEX_SPE_FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/atomtex_spe_file/include)
target_include_directories(atomtex_spe_file_cli PRIVATE
  ${ATOMTEX_SPE_FILE_INCLUDE_DIR}
)
