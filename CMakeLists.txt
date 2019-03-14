cmake_minimum_required(VERSION 3.9)

project(cingulata VERSION 0.3.0 LANGUAGES CXX)

option(ENABLE_UNITTEST "Enable unit tests using googletest" ON)
option(BUILD_DOC "Build documentation" ON)

set(USE_ABC ON CACHE BOOL INTERNAL)

add_subdirectory(submodules)
add_subdirectory(common)
add_subdirectory(optim)
add_subdirectory(runtime)

# include_directories(common/include)
# add_subdirectory(tests)



