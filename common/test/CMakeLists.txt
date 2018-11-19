cmake_minimum_required(VERSION 3.0)

include_directories(${INCLUDE_DIR})

set(ENABLE_UNITTEST ON CACHE BOOL "Enable unit tests using googletest")

if(ENABLE_UNITTEST)
  add_subdirectory(googletest EXCLUDE_FROM_ALL)

  # if gtest_SOURCE_DIR has been set
  if (gtest_SOURCE_DIR)
    set(GOOGLETEST_SOURCES
        unittest/test_ci_bit.cxx
        )

    add_executable(unittests ${GOOGLETEST_SOURCES})
    target_include_directories(unittests
      PRIVATE ${gtest_SOURCE_DIR}/include)
    target_link_libraries(unittests gtest_main common -lpthread)
    add_test(unittests unittests)

  else(gtest_SOURCE_DIR)
    message(WARNING "Cannot find googletest sources")
  endif(gtest_SOURCE_DIR)

endif(ENABLE_UNITTEST)

add_executable(simple_test simple_test.cxx)
target_link_libraries(simple_test common)
target_compile_options(simple_test PUBLIC -Wall)

add_executable(test_cpp_oper test_cpp_oper.cxx)
target_link_libraries(test_cpp_oper)
target_compile_options(test_cpp_oper PUBLIC -Wall)
