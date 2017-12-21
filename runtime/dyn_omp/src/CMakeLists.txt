cmake_minimum_required(VERSION 3.0)

set(SRCS 
    blif_circuit.cxx
    dyn_omp.cxx
    homomorphic_executor.cxx
    priority.cxx
    scheduler.cxx
    )

add_compile_options(-std=c++11 -Wall)

add_executable(dyn_omp ${SRCS})

target_include_directories(dyn_omp PRIVATE ../include)
target_link_libraries(dyn_omp fhe_fv ${LIBS})
