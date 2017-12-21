cmake_minimum_required(VERSION 3.0)

set(SRCS 
    bit-tracker.cxx
    circuit.cxx  
    node.cxx
    storage.cxx
    )

add_compile_options(-Wall)

add_library(generator ${SRCS})

target_include_directories(generator PUBLIC ../include)
