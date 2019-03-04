cmake_minimum_required(VERSION 3.0)

include_directories(${INCLUDE_DIR})

if(ENABLE_UNITTEST)
  # if gtest_SOURCE_DIR has been set
  if (gtest_SOURCE_DIR AND gmock_SOURCE_DIR)
    set(UNITTEST_SOURCES
        unittest/test_ci_bit.cxx
        unittest/test_ci_bit_vector.cxx
        unittest/test_ci_int.cxx
        unittest/test_io_name_vec.cxx
        unittest/test_int_op_gen_impl.cxx
        )

    add_executable(unittests ${UNITTEST_SOURCES})
    target_include_directories(unittests
      PRIVATE ${gtest_SOURCE_DIR}/include)
    target_include_directories(unittests
      PRIVATE ${gmock_SOURCE_DIR}/include)
    target_link_libraries(unittests gtest_main gmock_main common -lpthread)
    add_test(unittests unittests)

  else(gtest_SOURCE_DIR AND gmock_SOURCE_DIR)
    message(WARNING "Unittest compilation requested but googletest unavailable")
  endif(gtest_SOURCE_DIR AND gmock_SOURCE_DIR)

endif(ENABLE_UNITTEST)

add_executable(simple_test simple_test.cxx)
target_link_libraries(simple_test common)
target_compile_options(simple_test PUBLIC -Wall)

add_executable(test_cpp_oper test_cpp_oper.cxx)
target_link_libraries(test_cpp_oper)
target_compile_options(test_cpp_oper PUBLIC -Wall)
