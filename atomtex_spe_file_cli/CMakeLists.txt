find_package(Boost REQUIRED COMPONENTS program_options json)

add_executable(atomtex_spe_file_cli main.cpp)
target_link_libraries(atomtex_spe_file_cli PRIVATE
  Boost::program_options
  Boost::json
  atomtex_spe_file
)
