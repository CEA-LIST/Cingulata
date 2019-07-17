cmake_minimum_required(VERSION 3.9)

project(tests)

add_custom_target(tests_all ALL)

#############################################
# bfv tests
#############################################
if (USE_BFV)
  set(OPTIM_DIR ${CMAKE_CURRENT_BINARY_DIR}/../optim)
  set(RUNTIME_DIR ${CMAKE_CURRENT_BINARY_DIR}/../runtime)
  set(SCRIPT_DIR ${RUNTIME_DIR}/fhe_fv/script)

  if(NOT DEFINED MODEL)
    set(MODEL bkz_sieve)
  endif()

  if(NOT DEFINED MIN_SECU)
    set(MIN_SECU 128)
  endif()
  if(NOT DEFINED POLITIC) 
        set(POLITIC Cingulata_BFV)
  endif()


  #############################################
  # common interface tests
  #############################################
  add_subdirectory(bfv)
  add_dependencies(tests_all tests_bfv)

  #############################################
  # deprecated circuit frontend tests
  #############################################
  add_subdirectory(old_bfv)
  add_dependencies(tests_all tests_old_bfv)

endif(USE_BFV)


#############################################
# tfhe tests
#############################################
if(USE_TFHE)
  include_directories(${CMAKE_SOURCE_DIR}/runtime/tfhe/include)
  add_subdirectory(tfhe)
  add_dependencies(tests_all tests_tfhe)
endif(USE_TFHE)
