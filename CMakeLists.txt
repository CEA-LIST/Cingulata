cmake_minimum_required(VERSION 3.9)

project(cingulata VERSION 0.2.0 LANGUAGES CXX)

option(ENABLE_UNITTEST "Enable unit tests using googletest" ON)
option(BUILD_DOC "Build documentation" ON)

add_subdirectory(submodules)
add_subdirectory(common)

# add_subdirectory(circuit)
# add_subdirectory(optim)
# add_subdirectory(runtime)
# add_subdirectory(tests)



