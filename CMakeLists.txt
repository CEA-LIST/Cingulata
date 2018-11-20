cmake_minimum_required(VERSION 3.0)

project(cingulata VERSION 0.2.0 LANGUAGES CXX)

set(ENABLE_UNITTEST ON CACHE BOOL "Enable unit tests using googletest")

add_subdirectory(submodules)
add_subdirectory(common)

# add_subdirectory(circuit)
# add_subdirectory(optim)
# add_subdirectory(runtime)
# add_subdirectory(tests)



