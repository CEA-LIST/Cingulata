cmake_minimum_required(VERSION 3.0)

set(TEST_NAME old-intersection)

set(SRCS
    intersection.cxx
    )

set(FIRST_LIST_SIZE 5)
set(SECOND_LIST_SIZE 5)

set(BLIF_NAME ${TEST_NAME}.blif)
set(BLOP_NAME ${TEST_NAME}-opt.blif)

add_compile_options(-Wall -std=c++11 -Dfirst_list_size=${FIRST_LIST_SIZE} -Dsecond_list_size=${SECOND_LIST_SIZE} -Dblif_name="${BLIF_NAME}")

set(GEN_NAME ${TEST_NAME}-gen)

add_executable(${GEN_NAME} ${SRCS})

target_link_libraries(${GEN_NAME} common)

add_custom_command(OUTPUT ${BLIF_NAME}
  COMMAND ./${GEN_NAME} < /dev/null
  DEPENDS ${GEN_NAME})

add_custom_command(OUTPUT ${BLOP_NAME}
  COMMAND python3 ${OPTIM_DIR}/abc_optimize.py -i ${BLIF_NAME} -o ${BLOP_NAME} -v
  DEPENDS abc ${BLIF_NAME})


set(XML_PARAMS fhe_params.xml)
set(MUL_DEPTH_SCRIPT ${OPTIM_DIR}/graph_info.py)
set(GEN_PARAM_SCRIPT ${RUNTIME_DIR}/fhe_fv/script/genParams.py)

add_custom_command(OUTPUT ${XML_PARAMS}
  COMMAND python3 ${GEN_PARAM_SCRIPT} --lambda_p 128 --mult_depth `python3 ${MUL_DEPTH_SCRIPT} ${BLOP_NAME} --mult_depth_max` --cyclotomic_poly_index 1024 --output_xml ${XML_PARAMS}
  DEPENDS ${BLOP_NAME})

set(LAST_DEPEND ${XML_PARAMS})

add_custom_target(${TEST_NAME} ALL
  DEPENDS ${XML_PARAMS} runtime)

set(APPS_DIR ${CMAKE_BINARY_DIR}/apps)
set(CIRCUIT ${BLOP_NAME})
configure_file("run.sh.in" "run.sh" @ONLY)
file(COPY "plaintext/" DESTINATION "plaintext/" )
file(COPY "README.md" DESTINATION .)
