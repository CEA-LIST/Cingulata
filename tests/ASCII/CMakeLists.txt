cmake_minimum_required(VERSION 3.0)

set(TEST_NAME ASCII)

set(SRCS
    ${TEST_NAME}.cxx
    )

set(LIST_SIZE 10)
set(NB_CHARS 6)
set(BLIF_NAME ${TEST_NAME}.blif)
set(BLOP_NAME ${TEST_NAME}-opt.blif)

add_compile_options(-Wall -std=c++11 -Dnb_chars=${NB_CHARS} -Dlist_size=${LIST_SIZE} -Dblif_name="${BLIF_NAME}")

set(GEN_NAME ${TEST_NAME}-gen)

add_executable(${GEN_NAME} ${SRCS})

target_link_libraries(${GEN_NAME} common)

add_custom_command(OUTPUT ${BLIF_NAME}
  COMMAND ./${GEN_NAME}
  DEPENDS ${GEN_NAME})

add_custom_command(OUTPUT ${BLOP_NAME}
  COMMAND python3 ${OPTIM_DIR}/abc_optimize.py -i ${BLIF_NAME} -o ${BLOP_NAME} -v
  DEPENDS abc ${BLIF_NAME})

set(XML_PARAMS fhe_params.xml)
set(MUL_DEPTH_SCRIPT ${OPTIM_DIR}/graph_info.py)

add_custom_command(OUTPUT ${XML_PARAMS}
        COMMAND bash ${SCRIPT_DIR}/selectParams.sh ${TEST_NAME} `python3 ${MUL_DEPTH_SCRIPT} ${BLOP_NAME} --mult_depth_max` ${MODEL} ${MIN_SECU}
        DEPENDS ${BLOP_NAME})

add_custom_target(${TEST_NAME} ALL
  DEPENDS ${XML_PARAMS} runtime)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
set(CIRCUIT ${BLOP_NAME})

configure_file("search_fixed_length.sh.in" "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/search_fixed_length.sh" @ONLY)
configure_file("search_max_length.sh.in" "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/search_max_length.sh" @ONLY)

file(COPY "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/search_fixed_length.sh" DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
file(COPY "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/search_max_length.sh" DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

file(COPY "plaintext/countries" DESTINATION "plaintext" )
file(COPY "plaintext/countries${NB_CHARS}" DESTINATION "plaintext" )
file(COPY "README.md" DESTINATION .)
