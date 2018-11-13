cmake_minimum_required(VERSION 3.9)

project("Cingulata common" VERSION 0.2.0 LANGUAGES CXX)

option(BUILD_DOC "Build documentation" ON)

if (BUILD_DOC)
  find_package(Doxygen)

  if (DOXYGEN_FOUND)
    doxygen_add_docs(doc
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
  else (DOXYGEN_FOUND)
    message("Doxygen binary not found")
  endif (DOXYGEN_FOUND)
endif(BUILD_DOC)


set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)

