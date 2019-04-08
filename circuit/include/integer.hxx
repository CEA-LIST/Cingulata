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
#include "sliced-integer.hxx"

[[deprecated("this class will be removed in next major release")]]
 typedef SlicedInteger<int8_t>  Integer8;
[[deprecated("this class will be removed in next major release")]]
 typedef SlicedInteger<int16_t> Integer16;
[[deprecated("this class will be removed in next major release")]]
 typedef SlicedInteger<int32_t> Integer32;
[[deprecated("this class will be removed in next major release")]]
 typedef SlicedInteger<int64_t> Integer64;
[[deprecated("this class will be removed in next major release")]]
 typedef Integer8::Bit Bit;

#include <ci_context.hxx>
#include <bit_exec/tracker.hxx>

#define FINALIZE_CIRCUIT(name)                                                 \
  {                                                                            \
    cingulata::CiContext::get_bit_exec_t<cingulata::BitTracker>()              \
        ->export_blif(name);                                                   \
  }

#endif /* !defined(__INTEGER_H__) */


