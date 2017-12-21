cmake_minimum_required(VERSION 3.0)

project(dyn_omp)

find_package(Threads REQUIRED)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

find_package(Boost 1.58 REQUIRED COMPONENTS program_options graph)
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)

