cmake_minimum_required(VERSION 3.0)


set(SRCS
    bit_exec/interface.cxx
    bit_exec/bit_tracker.cxx
    )

include_directories(${INCLUDE_DIR})

add_library(common ${SRCS})

target_compile_options(common PRIVATE -std=c++11 -Wall)
