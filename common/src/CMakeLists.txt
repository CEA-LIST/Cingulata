cmake_minimum_required(VERSION 3.0)

set(SRCS
    bit_exec/interface_she.cxx
    bit_exec/tracker.cxx
    ci_bit.cxx
    ci_bit_vector.cxx
    ci_context.cxx
    ci_fncs.cxx
    ci_int.cxx
    slice.cxx
    int_op_gen/impl/adder.cxx
    int_op_gen/impl/dec.cxx
    int_op_gen/impl/equal.cxx
    int_op_gen/impl/lower.cxx
    int_op_gen/impl/multiplier.cxx
    int_op_gen/impl/mux.cxx
    int_op_gen/impl/negate.cxx
    int_op_gen/impl/sort.cxx
    int_op_gen/impl/operator.cxx
    int_op_gen/interface.cxx
    int_op_gen/mult_depth.cxx
    int_op_gen/impl/multi_inp_adder.cxx
    int_op_gen/size.cxx
    bit_exec/clear.cxx
    bit_exec/circuit/blif_io.cxx
    bit_exec/circuit/circuit.cxx
    bit_exec/parallel/scheduler.cxx
    bit_exec/parallel/worker.cxx
    )

add_library(common ${SRCS})

target_include_directories(common PUBLIC ${INCLUDE_DIR})
target_compile_options(common PRIVATE -Wall)

# link pthread
find_package(Threads REQUIRED)
target_link_libraries(common PUBLIC ${CMAKE_THREAD_LIBS_INIT})

# link fmt library
target_link_libraries(common PUBLIC fmt::fmt)

# uncomment desired logging level line - in release mode warning is default and in debug mode trace is default
# target_compile_definitions(common PRIVATE -DCINGU_LOG_LEVEL=CINGU_LOG_LEVEL_TRACE)
# target_compile_definitions(common PRIVATE -DCINGU_LOG_LEVEL=CINGU_LOG_LEVEL_DEBUG)
target_compile_definitions(common PRIVATE -DCINGU_LOG_LEVEL=CINGU_LOG_LEVEL_WARN)
# target_compile_definitions(common PRIVATE -DCINGU_LOG_LEVEL=CINGU_LOG_LEVEL_ERROR)
