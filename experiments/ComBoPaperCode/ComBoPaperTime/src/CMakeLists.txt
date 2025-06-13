set(TFHE_PREFIX /your/path/to/tfhe/lib)
set(APP_NAME executable)
set(MAIN ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(APP_SRCS
  ${CMAKE_CURRENT_LIST_DIR}/base_b_keyswitch.cpp
  ${CMAKE_CURRENT_LIST_DIR}/base_b_keyswitchkey.cpp
  ${CMAKE_CURRENT_LIST_DIR}/comBo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/FDFB_functions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Half-Torus.cpp
  ${CMAKE_CURRENT_LIST_DIR}/test_operations_FFT.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tota_functions.cpp
  )

set(APP_HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/base_b_keyswitch.h
  ${CMAKE_CURRENT_LIST_DIR}/base_b_keyswitchkey.h
  ${CMAKE_CURRENT_LIST_DIR}/comBo.h
  ${CMAKE_CURRENT_LIST_DIR}/FDFB_functions.h
  ${CMAKE_CURRENT_LIST_DIR}/Half-Torus.h
  ${CMAKE_CURRENT_LIST_DIR}/test_operations_FFT.h
  ${CMAKE_CURRENT_LIST_DIR}/tota_functions.h
  )

find_package(Threads REQUIRED)

add_executable(${APP_NAME} ${APP_SRCS} ${MAIN} ${APP_HEADERS})

include_directories(${TFHE_PREFIX}/src/include)

target_link_libraries(${APP_NAME} ${TFHE_PREFIX}/build/libtfhe/libtfhe-spqlios-avx.so)

target_link_libraries(${APP_NAME} Threads::Threads)

set_target_properties(${APP_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  )
