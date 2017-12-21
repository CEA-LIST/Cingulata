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
FILE=trivium

APPS_DIR=$CURR_DIR/../../apps/

mkdir -p input
rm -f input/*.ct

mkdir -p output
rm -f output/*.ct

# Generate keys
echo "FHE key generation"
$APPS_DIR/generate_keys

echo "Input encryption"
NR_THREADS=1

KEY_BIN='0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1'

IV_BIN='0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1'

echo "KEY encryption"
$APPS_DIR/encrypt --public-key fhe_key.pk -v --threads $NR_THREADS `$APPS_DIR/helper --bit-cnt 1 --prefix input/i_ --msb-first --start-idx 2 --idx-places 0  $KEY_BIN` 

echo "IV formatting"
$APPS_DIR/encrypt --public-key fhe_key.pk -v --threads $NR_THREADS --clear `$APPS_DIR/helper --bit-cnt 1 --prefix input/i_ --msb-first --start-idx 82 --idx-places 0  $IV_BIN`

echo "FHE execution"
time $APPS_DIR/dyn_omp $FILE'-opt.blif' --threads $NR_THREADS #-v > out.log

echo "Output decryption"
OUT_FILES=`ls -v output/*`
$APPS_DIR/helper --from-bin --bit-cnt 8 --msb-first `$APPS_DIR/decrypt --secret-key fhe_key.sk $OUT_FILES`

