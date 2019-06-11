cmake_minimum_required(VERSION 3.9)

set(test_names
      hello
      trivium
      cardio
      ipv4
      ipv6
      kreyvium
      ASCII
      membership
      multiplicity
      membership-partial
      wiretap
)

set(target_names "")
foreach (name IN ITEMS ${test_names})
  add_subdirectory(${name})
  list(APPEND target_names "bfv-${name}")
endforeach()

add_custom_target(tests_bfv ALL
  DEPENDS
    ${target_names}
)
