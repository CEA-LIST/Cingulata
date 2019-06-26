cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-hello-exec hello.cxx)
target_link_libraries(tfhe-hello-exec common tfhe_bit_exec)

add_executable(tfhe-hello-encrypt encrypt.cxx)
target_link_libraries(tfhe-hello-encrypt common tfhe_bit_exec)

add_executable(tfhe-hello-decrypt decrypt.cxx)
target_link_libraries(tfhe-hello-decrypt common tfhe_bit_exec)

add_custom_target(tfhe-hello
  DEPENDS
    tfhe-hello-exec
    tfhe-hello-encrypt
    tfhe-hello-decrypt
    tfhe
)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
