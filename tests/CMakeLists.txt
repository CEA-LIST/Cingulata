cmake_minimum_required(VERSION 3.0)

project(tests)

if(NOT DEFINED MODEL) 
        set(MODEL bkz_sieve)
endif()

if(NOT DEFINED MIN_SECU) 
        set(MIN_SECU 128)
endif()

#multiplicative depth

#4
add_subdirectory(flip)

#5
add_subdirectory(ipv4)
add_subdirectory(membership)

#6
add_subdirectory(ASCII)
add_subdirectory(intersection)

#7
add_subdirectory(hello)
add_subdirectory(ipv6)
add_subdirectory(sorting)

#8
add_subdirectory(wiretap)

#10
add_subdirectory(cardio)

#12
add_subdirectory(multiplicity)

#13
add_subdirectory(kreyvium)
add_subdirectory(trivium)







