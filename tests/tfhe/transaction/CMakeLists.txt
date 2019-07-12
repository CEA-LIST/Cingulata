cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-transaction-exec transaction.cxx)
target_link_libraries(tfhe-transaction-exec common tfhe_bit_exec)

add_executable(tfhe-transaction-encrypt encrypt.cxx)
target_link_libraries(tfhe-transaction-encrypt common tfhe_bit_exec)

add_executable(tfhe-transaction-decrypt decrypt.cxx)
target_link_libraries(tfhe-transaction-decrypt common tfhe_bit_exec)

add_custom_target(tfhe-transaction
  DEPENDS
    tfhe-transaction-exec
    tfhe-transaction-encrypt
    tfhe-transaction-decrypt
)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
