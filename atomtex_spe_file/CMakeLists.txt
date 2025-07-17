set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(atomtex_spe_file
  ${SRC_DIR}/measurement.cpp
  ${SRC_DIR}/spe_file.cpp
  ${SRC_DIR}/utf16le_file.cpp
)

target_include_directories(atomtex_spe_file PRIVATE ${INCLUDE_DIR})
