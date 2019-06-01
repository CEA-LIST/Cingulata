cmake_minimum_required(VERSION 3.0)

set(CINGU_PARAM_DIR ${CMAKE_SOURCE_DIR}/submodules/CinguParam)
configure_file("selectParams.sh.in" "selectParams.sh" @ONLY)

