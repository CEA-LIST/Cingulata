cmake_minimum_required(VERSION 3.9)

project(tests)

set(OPTIM_DIR ${CMAKE_CURRENT_BINARY_DIR}/../optim)
set(RUNTIME_DIR ${CMAKE_CURRENT_BINARY_DIR}/../runtime)

#############################################
# common interface tests
#############################################
set(test_names
  hello
  trivium
  cardio
  ipv4
  ipv6
  kreyvium
)
foreach (name IN ITEMS ${test_names})
  add_subdirectory(${name})
endforeach()

add_custom_target(tests ALL
  DEPENDS
    ${test_names}
)

#############################################
# deprecated circuit frontend tests
#############################################
add_subdirectory(old)

#############################################
# tfhe tests
#############################################
if(TFHE_EXEC)
  include_directories(${CMAKE_SOURCE_DIR}/runtime/tfhe/include)
  add_subdirectory(tfhe)
endif(TFHE_EXEC)

add_custom_target(tests_all ALL
  DEPENDS
    tests
    tests_old
    $<$<BOOL:TFHE_EXEC>:tests_tfhe>
)
