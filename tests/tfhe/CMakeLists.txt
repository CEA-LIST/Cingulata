cmake_minimum_required(VERSION 3.9)

set(test_names
      # hello
)

set(target_names "")
foreach (name IN ITEMS ${test_names})
  add_subdirectory(${name})
  list(APPEND target_names "tfhe-${name}")
endforeach()

add_custom_target(tests_tfhe ALL
  DEPENDS
    ${target_names}
)

