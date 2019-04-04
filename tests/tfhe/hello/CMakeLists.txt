cmake_minimum_required(VERSION 3.9)

add_executable(hello-tfhe hello.cxx)
target_link_libraries(hello-tfhe common exec_tfhe)
