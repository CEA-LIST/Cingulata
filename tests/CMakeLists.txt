cmake_minimum_required(VERSION 3.9)

project(tests)

# common interface
add_subdirectory(hello)
add_subdirectory(cardio)
add_subdirectory(ipv4)
add_subdirectory(ipv6)

add_custom_target(tests ALL
  DEPENDS
    hello
    cardio
    ipv4
    ipv6
)

# deprecated circuit frontend
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
