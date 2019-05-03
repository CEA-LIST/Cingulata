cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-hello hello.cxx)
target_link_libraries(tfhe-hello common tfhe_bit_exec)

add_executable(tfhe-hello-encrypt encrypt.cxx)
target_link_libraries(tfhe-hello-encrypt common tfhe_bit_exec)

add_executable(tfhe-hello-decrypt decrypt.cxx)
target_link_libraries(tfhe-hello-decrypt common tfhe_bit_exec)


set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
configure_file("run.sh.in" "run.sh" @ONLY)
