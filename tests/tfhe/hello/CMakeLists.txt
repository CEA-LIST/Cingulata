cmake_minimum_required(VERSION 3.9)

add_executable(tfhe-hello hello.cxx)
target_link_libraries(tfhe-hello common tfhe_bit_exec)
