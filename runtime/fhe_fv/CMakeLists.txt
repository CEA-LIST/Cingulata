cmake_minimum_required(VERSION 3.0)

project(fhe_fv)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(FLINT REQUIRED)
# message(STATUS "FLINT include dir is: ${FLINT_INCLUDE_DIR}")
# message(STATUS "FLINT library is: ${FLINT_LIBRARIES}")
link_libraries(${FLINT_LIBRARIES})
include_directories(${FLINT_INCLUDE_DIR})

find_package(GMP REQUIRED)
# message(STATUS "GMP include dir is: ${GMP_INCLUDE_DIR}")
# message(STATUS "GMP library is: ${GMP_LIBRARIES}")
link_libraries(${GMP_LIBRARIES})
include_directories(${GMP_INCLUDE_DIR})

find_package(MPFR REQUIRED)
# message(STATUS "MPFR include dir is: ${MPFR_INCLUDE_DIR}")
# message(STATUS "MPFR library is: ${MPFR_LIBRARIES}")
link_libraries(${MPFR_LIBRARIES})
include_directories(${MPFR_INCLUDE_DIR})

find_package(PUGIXML REQUIRED)
# message(STATUS "PUGIXML include dir is: ${PUGIXML_INCLUDE_DIR}")
# message(STATUS "PUGIXML library is: ${PUGIXML_LIBRARIES}")
link_libraries(${PUGIXML_LIBRARIES})
include_directories(${PUGIXML_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(script)

