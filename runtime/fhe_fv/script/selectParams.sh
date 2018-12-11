#!/bin/bash

# -*- coding: utf-8 -*-
#
#    (C) Copyright 2018 CEA LIST. All Rights Reserved.
#    Contributor(s): Cingulata team (formerly Armadillo team)
#
#    This software is governed by the CeCILL-C license under French law and
#    abiding by the rules of distribution of free software.  You can  use,
#    modify and/ or redistribute the software under the terms of the CeCILL-C
#    license as circulated by CEA, CNRS and INRIA at the following URL
#    "http://www.cecill.info".
#
#    As a counterpart to the access to the source code and  rights to copy,
#    modify and redistribute granted by the license, users are provided only
#    with a limited warranty  and the software's author,  the holder of the
#    economic rights,  and the successive licensors  have only  limited
#    liability.
#
#    The fact that you are presently reading this means that you have had
#    knowledge of the CeCILL-C license and that you accept its terms.
#



# callers: tests/<test-name>/CMakeLists.txt during make 

# positional parameters
TEST_NAME=$1
BLOP_NAME=$2
MODEL=$3
MIN_SECU=$4
MODE="static" 
# interactive mode indicates the user is ready to interact via yad to make a second choice if no parameter set is found with given constraints.
# static mode indicates the user prefers no interaction during compilation phase.  

function print() # syntax: print VAR1 ... VARN
{
        for  arg in "$@"
        do
            echo "$arg"="$(eval "echo \$$arg")"
        done
        
}

#directories
RUN_DIR=$(pwd)
cd ../../ || exit
BUILD_DIR=$(pwd)
cd .. || exit
BASE_DIR=$(pwd)

#Color text
DEFAULT_ZEN='\e[m'
CYAN_WARNING='\e[0;36m'
RED_ERROR='\e[0;31m'


if [ ! "$(ls -A "$BASE_DIR"/CinguParam)" ] ; then
   echo -e "${RED_ERROR}The submodule CinguParam is not initialized.${DEFAULT_ZEN}"
   exit 1
fi

PARAM_DIR=${BASE_DIR}/CinguParam/storeParam

#print BASE_DIR PARAM_DIR RUN_DIR BUILD_DIR


XML_PARAMS=fhe_params.xml 
HEAD_ID=$(git ls-remote https://bitbucket.org/malb/lwe-estimator.git HEAD | awk '{print $1}' | cut -c-7 ) # to determine lwe-estimator HEAD commit ID  
COMMIT_ID="$(awk '{w=$1} END{print w}' "${PARAM_DIR}"/commit.log)" # to determine last commit ID in database

#compute circuit multiplicative depth
MUL_DEPTH_SCRIPT=${BUILD_DIR}/optim/graph_info.py
cd "${RUN_DIR}" || exit
MUL_DEPTH=$(python3 "${MUL_DEPTH_SCRIPT}" "${BLOP_NAME}" --mult_depth_max)


DESIRED_PARAMS="${PARAM_DIR}/${HEAD_ID}/${MUL_DEPTH}_${MODEL}_${MIN_SECU}"




# select static choice parameters with:
# * mul.depth or mul. depth + 1
# * given bkz reduction cost model
# * secu in the interval [lambda-8,lambda+64]
static_choice()
{
  cd "${PARAM_DIR}/$1/" || exit
  for file in * 
  do
        depth=$(echo "${file}" | awk -F'[_]' '{print $1}')  # print multiplicative depth
        model=$(echo "${file}" | sed 's/[0-9]//g' | sed 's/^_//' | sed 's/_$//') # print bkz reduction cost model
        secu=$(echo "${file}" | awk -F'[_]' '{print $NF}') # print approximated security
        
        if [ $((depth)) -eq $((MUL_DEPTH))  ] && [ "${secu}" -ge $((MIN_SECU)) ] && [ "${secu}" -le $((MIN_SECU+64)) ]  && [ "${model}" = "${MODEL}" ]
        then
                sublist+=( "${file}" )
        fi
  done  
  echo DESIRED_PARAMS="$(echo "${DESIRED_PARAMS}"| rev | cut -d/ -f1 | rev)"
  if [ ${#sublist[@]} -ne 0 ] 
  then 
    STATIC_CHOICE=${sublist[0]} # choose arbitrarily first element in sublist.
else
    echo -e "${CYAN_WARNING}No parameter found with static mode. Automatically switch to interactive mode.${DEFAULT_ZEN}"
    MODE=interactive
    PREVIOUS_MODE=static
fi
        
    
}

print TEST_NAME
if [ -f "${DESIRED_PARAMS}" ]
then
        cp -v "${DESIRED_PARAMS}" "${RUN_DIR}/${XML_PARAMS}" 2>/dev/null 
else
        print MODE 
        if [ $MODE = "static" ]
        then
                if [ -d "${PARAM_DIR}/${HEAD_ID}" ]
                then
                        echo -e "${CYAN_WARNING}No parameter set found for test ${TEST_NAME} with given triplet (multiplicative_depth=${MUL_DEPTH},reduction_cost_model=${MODEL},desired_security_level=${MIN_SECU}).${DEFAULT_ZEN}"
                        echo "Compute alternative choice..."
                        static_choice "${HEAD_ID}"
                else
                        echo -e "${CYAN_WARNING}The parameters with lwe-estimator commit ${HEAD_ID} are not in the database.${DEFAULT_ZEN}"
                        static_choice "${COMMIT_ID}"
                fi        
                if [ -n "${STATIC_CHOICE}" ]
                then
                        echo "STATIC_CHOICE=${STATIC_CHOICE} (last commit=${COMMIT_ID})"
                        cp -v "${STATIC_CHOICE}" "${RUN_DIR}/${XML_PARAMS}" 2>/dev/null
                fi        
        fi
        if [ $MODE = "interactive" ]
        then
                if [ -d "${PARAM_DIR}/${HEAD_ID}" ]
                then
                        if [ -z "${PREVIOUS_MODE}" ]
                        then
                                echo -e "${CYAN_WARNING}No parameter set found for test ${TEST_NAME} with given triplet (multiplicative_depth=${MUL_DEPTH},reduction_cost_model=${MODEL},desired_security_level=${MIN_SECU}).${DEFAULT_ZEN}"
                        fi
                        read -p "Press <ENTER> to select a parameter set." -r
                        cd "${PARAM_DIR}/${HEAD_ID}/" || exit
                        INTERACTIVE_CHOICE=$(yad --text "Multiplicative depth of circuit ${TEST_NAME} is ${MUL_DEPTH}." --file-selection --title "Filename syntax: multiplicative-depth_BKZ-reduction-cost-model_desired-security-level." --file-filter="${MUL_DEPTH}_* $((MUL_DEPTH+1))_* $((MUL_DEPTH+2))_*")
                        print INTERACTIVE_CHOICE 
                        if [ -f "${INTERACTIVE_CHOICE}" ] && [ ! -z "${INTERACTIVE_CHOICE}" ]
                        then
                                cp -v "${INTERACTIVE_CHOICE}" "${RUN_DIR}/${XML_PARAMS}" 2>/dev/null
                        else
                                echo -e "${RED_ERROR}Invalid choice. You can generate suitable parameter sets using CinguParam module. ${DEFAULT_ZEN}" # It happens for L>20 since there is no corresponding paramset in the database.
                                exit 0
                        fi   
                else
                        if [ -z "${PREVIOUS_MODE}" ]
                        then                
                                echo -e "${CYAN_WARNING}The parameters with lwe-estimator commit ${HEAD_ID} are not in the database.${DEFAULT_ZEN}"
                        fi
                        PS3='Please enter your choice: '
                        options=("Select a parameter set in the database (last commit=${COMMIT_ID})" "Update the database (dependency: sagemath)" "Quit")
                        select REPLY in "${options[@]}"
                        do
                            case ${REPLY} in
                                "Select a parameter set in the database (last commit=${COMMIT_ID})")
                                    cd "${PARAM_DIR}/${COMMIT_ID}/" || exit
                                    INTERACTIVE_CHOICE=$(yad --text "Multiplicative depth of circuit ${TEST_NAME} is ${MUL_DEPTH}." --file-selection --title "Filename syntax: multiplicative-depth_BKZ-reduction-cost-model_desired-security-level." --file-filter="${MUL_DEPTH}_* $((MUL_DEPTH+1))_* $((MUL_DEPTH+2))_*" )
                                    print INTERACTIVE_CHOICE COMMIT_ID
                                    if [ -f "${INTERACTIVE_CHOICE}" ] && [ ! -z "${INTERACTIVE_CHOICE}" ]
                                    then
                                        cp -v "${INTERACTIVE_CHOICE}" "${RUN_DIR}/${XML_PARAMS}" 2>/dev/null
                                    else
                                        echo -e "${RED_ERROR}Invalid choice. You can generate suitable parameter sets using CinguParam module. ${DEFAULT_ZEN}" # It happens for L>20 since there is no corresponding paramset in the database.
                                        exit 0
                                    fi
                                    break
                                    ;;
                                "Update the database (dependency: sagemath)")
                                    cd "${BASE_DIR}/CinguParam/generateParam"  \
                                    && parallel  --header : --results ${PARAM_DIR}/${HEAD_ID} bash updateParam.sh {1} {2} {3} {4} ${HEAD_ID} ::: mult_depth $(seq 20) ::: min_secu 80 128 192 ::: model "bkz_enum" "bkz_sieve" "core_sieve" "q_core_sieve" ::: gen_method "wordsizeinc" "bitsizeinc" \
                                    && bash renameParam.sh ${PARAM_DIR}/${HEAD_ID} 80 128 192  \
                                    && echo "${HEAD_ID}" "$(date)" >> "${PARAM_DIR}/commit.log"
                                    break
                                    ;;
                                "Quit")
                                    break
                                    ;;
                                *) echo "invalid option ${REPLY}";;
                            esac
                        done                
                fi
        fi
fi



   
