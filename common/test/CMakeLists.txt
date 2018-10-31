cmake_minimum_required(VERSION 3.0)

set(SRCS
    simple_test.cxx
    )

include_directories(${INCLUDE_DIR})

add_executable(simple_test simple_test.cxx)
target_link_libraries(simple_test common)
target_compile_options(simple_test PUBLIC -std=c++11 -Wall)
