cmake_minimum_required(VERSION 3.9)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)

add_subdirectory(fhe_fv)

add_subdirectory(fhe_apps)

add_subdirectory(dyn_omp)

if (TFHE_EXEC)
  add_subdirectory(tfhe)
endif(TFHE_EXEC)
