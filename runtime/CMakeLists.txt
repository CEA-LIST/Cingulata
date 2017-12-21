cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/apps)

add_subdirectory(fhe_fv)

add_subdirectory(fhe_apps)

add_subdirectory(dyn_omp)
