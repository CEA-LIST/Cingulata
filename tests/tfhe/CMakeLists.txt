cmake_minimum_required(VERSION 3.9)

FILE(GLOB test_names [^_]*)

set(target_names "")
foreach (child ${test_names})
  if(IS_DIRECTORY ${child})
    get_filename_component(name ${child} NAME)
    add_subdirectory(${name})
    list(APPEND target_names "tfhe-${name}")
  endif()
endforeach()

add_custom_target(tests_tfhe ALL
  DEPENDS
    ${target_names}
)