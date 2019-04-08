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
FILE=sorting

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

$APPS_DIR/encrypt -v --threads $NR_THREADS `$APPS_DIR/helper --bit-cnt 8 42 7 37 2`

echo "FHE execution..."
$APPS_DIR/dyn_omp $FILE'-opt.blif' --threads $NR_THREADS

echo "Output decryption"
#decrypt 8-bit output
OUT_FILES=`ls -v output/*`
$APPS_DIR/helper --from-bin --bit-cnt 8  `$APPS_DIR/decrypt  $OUT_FILES`





