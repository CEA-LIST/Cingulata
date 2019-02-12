cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

set(SRCS
    bit_exec/interface.cxx
    bit_exec/tracker.cxx
    ci_bit.cxx
    ci_int.cxx
    ci_bit_vector.cxx
    int_op_gen/interface.cxx
    int_op_gen/impl/operator.cxx
    int_op_gen/impl/adder.cxx
    int_op_gen/impl/lower.cxx
    int_op_gen/impl/negate.cxx
    int_op_gen/impl/not_equal.cxx
    )

include_directories(${INCLUDE_DIR})

add_library(common ${SRCS})

target_compile_options(common PRIVATE -Wall)
