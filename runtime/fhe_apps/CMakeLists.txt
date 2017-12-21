cmake_minimum_required(VERSION 3.0)

project(fhe_apps)

find_package(OpenMP) #flags in var ${OpenMP_CXX_FLAGS} 

find_package(Boost 1.58 REQUIRED COMPONENTS program_options)
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
