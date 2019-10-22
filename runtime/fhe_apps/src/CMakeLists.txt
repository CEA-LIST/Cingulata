cmake_minimum_required(VERSION 3.0)

add_executable(encrypt encrypt.cxx)
target_link_libraries(encrypt fhe_fv_bit_exec)
set_target_properties(encrypt PROPERTIES
                    COMPILE_FLAGS ${OpenMP_CXX_FLAGS}
                    LINK_FLAGS ${OpenMP_CXX_FLAGS})


add_executable(decrypt decrypt.cxx)
target_link_libraries(decrypt fhe_fv_bit_exec)
set_target_properties(decrypt PROPERTIES
                    COMPILE_FLAGS ${OpenMP_CXX_FLAGS}
                    LINK_FLAGS ${OpenMP_CXX_FLAGS})


add_executable(generate_keys generate_keys.cxx)
target_link_libraries(generate_keys fhe_fv)


add_executable(pack pack.cxx)
target_link_libraries(pack fhe_fv_bit_exec)
set_target_properties(pack PROPERTIES
                    COMPILE_FLAGS ${OpenMP_CXX_FLAGS}
                    LINK_FLAGS ${OpenMP_CXX_FLAGS})

add_executable(helper helper.cxx)
target_link_libraries(helper)


add_executable(blif_exec blif_exec.cxx)
target_link_libraries(blif_exec fhe_fv_bit_exec)

find_package(Boost 1.58 REQUIRED COMPONENTS program_options)
target_link_libraries(blif_exec ${Boost_LIBRARIES})
target_include_directories(blif_exec PRIVATE ${Boost_INCLUDE_DIRS})


add_custom_target(fhe_apps
  DEPENDS encrypt decrypt generate_keys pack helper blif_exec)
