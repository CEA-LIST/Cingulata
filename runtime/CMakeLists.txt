cmake_minimum_required(VERSION 3.9)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)

if (USE_BFV)
  add_subdirectory(fhe_fv)
  add_subdirectory(fhe_apps)
  add_subdirectory(dyn_omp)
  add_custom_target(runtime
    DEPENDS fhe_apps dyn_omp)
endif (USE_BFV)

if (USE_TFHE)
  add_subdirectory(tfhe)
endif(USE_TFHE)
