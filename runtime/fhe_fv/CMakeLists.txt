cmake_minimum_required(VERSION 3.0)

project(fhe_fv)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(FLINT REQUIRED)
link_libraries(${FLINT_LIBRARIES})
include_directories(${FLINT_INCLUDE_DIR})

find_package(GMP REQUIRED)
link_libraries(${GMP_LIBRARIES})
include_directories(${GMP_INCLUDE_DIR})

find_package(MPFR REQUIRED)
link_libraries(${MPFR_LIBRARIES})
include_directories(${MPFR_INCLUDE_DIR})

find_package(PUGIXML REQUIRED)
link_libraries(${PUGIXML_LIBRARIES})
include_directories(${PUGIXML_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(script)

