cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-trans1-exec transaction.cxx)
target_link_libraries(tfhe-trans1-exec common tfhe_bit_exec)

add_executable(tfhe-trans1-encrypt encrypt.cxx)
target_link_libraries(tfhe-trans1-encrypt common tfhe_bit_exec)

add_executable(tfhe-trans1-decrypt decrypt.cxx)
target_link_libraries(tfhe-trans1-decrypt common tfhe_bit_exec)


add_custom_target(tfhe-trans1
  DEPENDS
    tfhe-keygen
    tfhe-trans1-exec
    tfhe-trans1-encrypt
    tfhe-trans1-decrypt
)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
