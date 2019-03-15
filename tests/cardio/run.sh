#!/bin/bash

#
#    (C) Copyright 2017 CEA LIST. All Rights Reserved.
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
#

CURR_DIR=$PWD
FILE=cardio

APPS_DIR=$CURR_DIR/../../apps/

mkdir -p input
rm -f input/*.ct

mkdir -p output
rm -f output/*.ct

# Generate keys
echo "FHE key generation"
$APPS_DIR/generate_keys

echo "Input formatting & encryption"
NR_THREADS=1

#encrypt client data
$APPS_DIR/helper --bit-cnt 5 --prefix %flags_        --start-idx 0 --suffix '' 254 > clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %age_          --start-idx 0 --suffix '' 229 >> clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %hdl_          --start-idx 0 --suffix '' 211 >> clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %height_       --start-idx 0 --suffix '' 139 >> clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %weight_       --start-idx 0 --suffix '' 12 >> clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %physical_act_ --start-idx 0 --suffix '' 6 >> clear_data.data
$APPS_DIR/helper --bit-cnt 8 --prefix %drinking_     --start-idx 0 --suffix '' 193  >> clear_data.data
sed -i 's/%/\n/g' clear_data.data
sed -i '/^$/d' clear_data.data

# encrypt 7*8-bit kreyvium ciphered inputs and homomorphically mined kreyvium key
KS=(241 210 225 219 92 43 197)
for (( i = 0; i < ${#KS[@]}; i++ )); do
  TMP=`$APPS_DIR/helper --bit-cnt 8 --prefix "input/ks_"$i"_" --start-idx 0 ${KS[i]}`
  $APPS_DIR/encrypt --public-key fhe_key.pk -v --threads $NR_THREADS $TMP
done

echo "FHE execution"
time $APPS_DIR/dyn_omp $FILE'-opt.blif' --threads $NR_THREADS --clear-inps clear_data.data -v

echo "Output decryption"
OUT_FILES=`ls -v output/*`
$APPS_DIR/helper --from-bin --bit-cnt 8 --msb-first `$APPS_DIR/decrypt --secret-key fhe_key.sk $OUT_FILES`


