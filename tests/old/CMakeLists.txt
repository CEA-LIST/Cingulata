cmake_minimum_required(VERSION 3.9)

project(tests_old)

add_subdirectory(kreyvium)
add_subdirectory(flip)
add_subdirectory(ASCII)
add_subdirectory(intersection)
add_subdirectory(sorting)
add_subdirectory(membership)
add_subdirectory(multiplicity)
add_subdirectory(trivium)
add_subdirectory(wiretap)

add_custom_target(tests_old ALL
  DEPENDS
    kreyvium
    flip
    ASCII
    intersection
    sorting
    membership
    multiplicity
    trivium
    wiretap
)
