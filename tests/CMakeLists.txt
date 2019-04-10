cmake_minimum_required(VERSION 3.9)

project(tests)

set(OPTIM_DIR ${CMAKE_CURRENT_BINARY_DIR}/../optim)
set(RUNTIME_DIR ${CMAKE_CURRENT_BINARY_DIR}/../runtime)

# common interface
add_subdirectory(hello)
add_subdirectory(trivium)
add_subdirectory(cardio)
add_subdirectory(ipv4)
add_subdirectory(ipv6)

add_custom_target(tests ALL
  DEPENDS
    hello
    trivium
    cardio
    ipv4
    ipv6
)

# deprecated circuit frontend tests
add_subdirectory(old)

add_custom_target(tests_all ALL
  DEPENDS
    tests
    tests_old
)
