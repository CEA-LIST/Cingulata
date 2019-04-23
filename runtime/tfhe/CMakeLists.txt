cmake_minimum_required(VERSION 3.9)

set(SRCS
    src/tfhe_bit_exec.cxx
    )

add_library(tfhe_bit_exec ${SRCS})

target_include_directories(tfhe_bit_exec PUBLIC ./include)
target_include_directories(tfhe_bit_exec PUBLIC ${COMMON_INCLUDE_DIR})
target_include_directories(tfhe_bit_exec PUBLIC ${TFHE_INCLUDE_DIR})
target_link_libraries(tfhe_bit_exec ${TFHE_LIBRARIES})
