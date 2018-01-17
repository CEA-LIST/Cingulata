cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

ExternalProject_Add(
    abc
    HG_REPOSITORY https://bitbucket.org/alanmi/abc
    HG_TAG 3414d13
		UPDATE_COMMAND ""
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/abc
		INSTALL_COMMAND ""
)

ExternalProject_Get_Property(abc BINARY_DIR)
ExternalProject_Get_Property(abc SOURCE_DIR)

set(ABC_PATH ${BINARY_DIR})
set(ABC_SRC_PATH ${SOURCE_DIR})
configure_file("abc_optimize.py.in" "abc_optimize.py" @ONLY)

configure_file("lib_red.genlib" "lib_red.genlib" COPYONLY)
configure_file("graph_info.py" "graph_info.py" COPYONLY)
configure_file("utils.py" "utils.py" COPYONLY)
