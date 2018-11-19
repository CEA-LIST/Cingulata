cmake_minimum_required(VERSION 3.0)

include_directories(${INCLUDE_DIR})

add_executable(simple_test simple_test.cxx)
target_link_libraries(simple_test common)
target_compile_options(simple_test PUBLIC -Wall)

add_executable(test_cpp_oper test_cpp_oper.cxx)
target_link_libraries(test_cpp_oper)
target_compile_options(test_cpp_oper PUBLIC -Wall)
