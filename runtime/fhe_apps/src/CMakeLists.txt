cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++11 -Wall)

add_executable(encrypt encrypt.cxx)
target_link_libraries(encrypt fhe_fv)
set_target_properties(encrypt PROPERTIES 
                    COMPILE_FLAGS ${OpenMP_CXX_FLAGS} 
                    LINK_FLAGS ${OpenMP_CXX_FLAGS})


add_executable(decrypt decrypt.cxx)
target_link_libraries(decrypt fhe_fv)
set_target_properties(decrypt PROPERTIES 
                    COMPILE_FLAGS ${OpenMP_CXX_FLAGS} 
                    LINK_FLAGS ${OpenMP_CXX_FLAGS})


add_executable(generate_keys generate_keys.cxx)
target_link_libraries(generate_keys fhe_fv)


add_executable(pack pack.cxx)
target_link_libraries(pack fhe_fv)


add_executable(helper helper.cxx)
target_link_libraries(helper)
