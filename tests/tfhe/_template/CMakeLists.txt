cmake_minimum_required(VERSION 3.9)

set(NAME "@NAME@") # project name

add_executable(tfhe-${NAME}-exec main.cxx)
target_link_libraries(tfhe-${NAME}-exec common tfhe_bit_exec)

add_executable(tfhe-${NAME}-encrypt encrypt.cxx)
target_link_libraries(tfhe-${NAME}-encrypt common tfhe_bit_exec)

add_executable(tfhe-${NAME}-decrypt decrypt.cxx)
target_link_libraries(tfhe-${NAME}-decrypt common tfhe_bit_exec)

add_custom_target(tfhe-${NAME}
  DEPENDS
    tfhe-${NAME}-exec
    tfhe-${NAME}-encrypt
    tfhe-${NAME}-decrypt
    tfhe
)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
