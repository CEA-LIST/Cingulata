cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

set(SRCS
    bit_exec/interface.cxx
    bit_exec/tracker.cxx
    ci_bit.cxx
    ci_int.cxx
    )

include_directories(${INCLUDE_DIR})

add_library(common ${SRCS})

target_compile_options(common PRIVATE -Wall)
