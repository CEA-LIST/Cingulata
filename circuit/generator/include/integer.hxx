/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team (formerly Armadillo team)

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

/* prevent multiple inclusions */
#if !defined(__INTEGER_H__)
#define __INTEGER_H__

/* compiler includes */
#include <stdint.h>

/* compilation of the program, integer types are sliced integer types */
#include <bit-tracker.hxx>
#include <sliced-integer.hxx>
typedef SlicedInteger<BitTracker,int8_t>  Integer8;
typedef SlicedInteger<BitTracker,int16_t> Integer16;
typedef SlicedInteger<BitTracker,int32_t> Integer32;
typedef SlicedInteger<BitTracker,int64_t> Integer64;
typedef BitTracker Bit;

#define FINALIZE_CIRCUIT(name) \
  { \
    ofstream f(name); \
    BitTracker::record().blif(f); \
    f.close(); \
  }

#endif /* !defined(__INTEGER_H__) */

