cmake_minimum_required(VERSION 3.9)

set(SUBMODULES_PATH "${PROJECT_SOURCE_DIR}/submodules")

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # update googletest
  if(INIT_GOOGLETEST_MODULE)
    message(STATUS "Submodule googletest update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULES_PATH}/googletest
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    add_subdirectory(googletest EXCLUDE_FROM_ALL)
  endif(INIT_GOOGLETEST_MODULE)

  # update ABC
  if(INIT_ABC_MODULE)
    message(STATUS "Submodule ABC update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULES_PATH}/abc
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    set(ABC_PATH ${SUBMODULES_PATH}/abc CACHE INTERNAL "" FORCE)

    #add build target
    add_custom_command(
      OUTPUT ${ABC_PATH}/abc
      COMMAND make ABC_USE_NO_READLINE=1
      WORKING_DIRECTORY ${ABC_PATH}
    )
    add_custom_target(abc DEPENDS ${ABC_PATH}/abc)

  endif(INIT_ABC_MODULE)

  # update CinguParam
  if(INIT_CINGU_PARAM_MODULE)
    message(STATUS "Submodule CinguParam update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULES_PATH}/CinguParam
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    set(CINGU_PARAM_PATH ${SUBMODULES_PATH}/CinguParam CACHE INTERNAL "" FORCE)
  endif(INIT_CINGU_PARAM_MODULE)
endif()
