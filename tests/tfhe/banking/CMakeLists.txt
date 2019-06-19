cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-banking-exec banking.cxx)
target_link_libraries(tfhe-banking-exec common tfhe_bit_exec)

add_executable(tfhe-banking-encrypt encrypt.cxx)
target_link_libraries(tfhe-banking-encrypt common tfhe_bit_exec)

add_executable(tfhe-banking-decrypt decrypt.cxx)
target_link_libraries(tfhe-banking-decrypt common tfhe_bit_exec)

add_custom_target(tfhe-banking
  DEPENDS
    tfhe-banking-exec
    tfhe-banking-encrypt
    tfhe-banking-decrypt
)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
