cmake_minimum_required(VERSION 3.0)

set(SRCS 
    ciphertext.cxx
    encdec.cxx
    fhe_params.cxx
    keygen.cxx
    keys_all.cxx
    keys_share.cxx
    normal.cxx
    polyring.cxx
    rand_polynom.cxx
    uniform.cxx
    )

add_compile_options(-Wall)

add_library(fhe_fv ${SRCS})

target_include_directories(fhe_fv PUBLIC ../include)