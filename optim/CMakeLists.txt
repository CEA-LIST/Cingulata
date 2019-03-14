cmake_minimum_required(VERSION 3.9)

configure_file("abc_optimize.py.in" "abc_optimize.py" @ONLY)

configure_file("lib_red.genlib" "lib_red.genlib" COPYONLY)
configure_file("graph_info.py" "graph_info.py" COPYONLY)
configure_file("utils.py" "utils.py" COPYONLY)
