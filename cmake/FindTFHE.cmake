find_path(
  TFHE_INCLUDE_DIR
  tfhe.h
  PATHS ${TFHE_PATH}/src/include
  NO_DEFAULT_PATH
)

find_library(
  TFHE_LIBRARIES
  libtfhe-spqlios-avx.so
  PATHS ${TFHE_PATH}/build/libtfhe
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(TFHE DEFAULT_MSG TFHE_INCLUDE_DIR TFHE_LIBRARIES)

message(STATUS "\tinclude : ${TFHE_INCLUDE_DIR}")
message(STATUS "\tlib     : ${TFHE_LIBRARIES}")

mark_as_advanced(TFHE_INCLUDE_DIR TFHE_LIBRARIES)
