cmake_minimum_required(VERSION 3.0)

set(SRCS
    ciphertext.cxx
    encdec.cxx
    fhe_params.cxx
    normal.cxx
    polyring.cxx
    rand_polynom.cxx
    uniform.cxx
    secret_key.cxx
    public_key.cxx
    )

add_compile_options(-Wall)

add_library(fhe_fv ${SRCS})

target_include_directories(fhe_fv PUBLIC ../include)


set(SRCS
    bfv_bit_exec.cxx
    )

add_library(fhe_fv_bit_exec ${SRCS})

target_include_directories(fhe_fv_bit_exec PRIVATE ../include)
target_include_directories(fhe_fv_bit_exec PUBLIC ${COMMON_INCLUDE_DIR})
target_link_libraries(fhe_fv_bit_exec fhe_fv common)
