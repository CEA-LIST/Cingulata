cmake_minimum_required(VERSION 3.9)

find_package(Git QUIET)

set(SUBMODULES_PATH "${PROJECT_SOURCE_DIR}/submodules")

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  option(GIT_SUBMODULE "Check submodules during build" ON)

  if(GIT_SUBMODULE)

    message(STATUS "Submodule googletest update")

    # update googletest
    if(ENABLE_UNITTEST)
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULES_PATH}/googletest
                      RESULT_VARIABLE GIT_SUBMOD_RESULT)
      if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
      endif()

      add_subdirectory(googletest EXCLUDE_FROM_ALL)
    endif(ENABLE_UNITTEST)

    # update ABC
    if(USE_ABC)
      message(STATUS "Submodule ABC update")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${SUBMODULES_PATH}/abc
                      RESULT_VARIABLE GIT_SUBMOD_RESULT)
      if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
      endif()

      set(ABC_PATH ${SUBMODULES_PATH}/abc CACHE INTERNAL "" FORCE)

      #add build target
      add_custom_target(
        abc
        COMMAND make ABC_USE_NO_READLINE=1
        DEPENDS ${ABC_PATH}
        WORKING_DIRECTORY ${ABC_PATH}
      )

    endif(USE_ABC)
  endif()
endif()
