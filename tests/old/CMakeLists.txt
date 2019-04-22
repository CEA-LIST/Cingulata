cmake_minimum_required(VERSION 3.9)

set(test_names
      kreyvium
      flip
      ASCII
      intersection
      sorting
      membership
      multiplicity
      trivium
      wiretap
)

set(target_names "")
foreach (name IN ITEMS ${test_names})
  add_subdirectory(${name})
  list(APPEND target_names "old-${name}")
endforeach()

add_custom_target(tests_old ALL
  DEPENDS
    ${target_names}
)

