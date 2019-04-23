cmake_minimum_required(VERSION 3.9)

project(cingulata VERSION 0.3.0 LANGUAGES CXX)

option(ENABLE_UNITTEST "Enable unit tests using googletest" ON)
option(BUILD_DOC "Build documentation" ON)
option(BUILD_ONLY_COMMON "Build only common interface (when only unittests are needed)" OFF)
set(USE_ABC ON CACHE BOOL INTERNAL)
set(TFHE_PATH "" CACHE PATH "TFHE library path")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(TFHE_EXEC OFF)
if (NOT ("${TFHE_PATH}" STREQUAL ""))
  find_package(TFHE REQUIRED)
  set(TFHE_EXEC ON)
endif()

set(COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/include)

add_subdirectory(submodules)
add_subdirectory(common)

message("ENABLE_UNITTEST    " ${ENABLE_UNITTEST})
message("BUILD_DOC          " ${BUILD_DOC})
message("BUILD_ONLY_COMMON  " ${BUILD_ONLY_COMMON})
message("USE_ABC            " ${USE_ABC})
message("TFHE_EXEC          " ${TFHE_EXEC})

if(NOT ${BUILD_ONLY_COMMON})
  add_subdirectory(optim)
  add_subdirectory(runtime)

  include_directories(${COMMON_INCLUDE_DIR})
  add_subdirectory(tests)
endif()


