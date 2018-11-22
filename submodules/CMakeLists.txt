cmake_minimum_required(VERSION 3.9)

if(ENABLE_UNITTEST)
  add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif(ENABLE_UNITTEST)
