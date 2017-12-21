find_path(GMP_INCLUDE_DIR gmp.h)
find_library(GMP_LIBRARIES NAMES gmp)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GMP DEFAULT_MSG GMP_INCLUDE_DIR GMP_LIBRARIES)

mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARIES)