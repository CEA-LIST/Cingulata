cmake_minimum_required(VERSION 3.9)

set(SRCS
    src/tfhe_bit_exec.cxx
    )

add_library(tfhe_bit_exec ${SRCS})

target_include_directories(tfhe_bit_exec PUBLIC ./include)
target_include_directories(tfhe_bit_exec PUBLIC ${COMMON_INCLUDE_DIR})
target_include_directories(tfhe_bit_exec PUBLIC ${TFHE_INCLUDE_DIR})
target_link_libraries(tfhe_bit_exec ${TFHE_LIBRARIES})

find_package(Boost 1.58 REQUIRED COMPONENTS program_options)

add_executable(tfhe-keygen src/keygen.cxx)
target_include_directories(tfhe-keygen PUBLIC ${TFHE_INCLUDE_DIR})
target_include_directories(tfhe-keygen PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(tfhe-keygen ${TFHE_LIBRARIES})
target_link_libraries(tfhe-keygen ${Boost_LIBRARIES})

add_custom_target(tfhe ALL
  DEPENDS
    tfhe_bit_exec
    tfhe-keygen
)


