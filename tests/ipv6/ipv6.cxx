/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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

/**
 * IPv6 equality test, each IPv6 is represented by eight 16-bit integers.
 **/

#include <cstdint>

/* local includes */
#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {

    /* Set context to bit tracker and multiplicative depth minimized integer
    * operations */
    CiContext::set_config(new BitTracker(), new IntOpGenDepth());

    /** IPv6 can be splitted into 8 integers of 2 octets. **/

    vector<CiInt> IP1(8,{CiInt::s16});
    vector<CiInt> IP2(8,{CiInt::s16});
    CiBit answer;

    for (int i = 0; i < 8; i++)
        IP1[i].read("a_" + to_string(i));

    for (int i = 0; i < 8; i++)
        IP2[i].read("b_" + to_string(i));

    /** Bitwise equality test **/

    answer = (((IP1[0] == IP2[0]) * (IP1[1] == IP2[1]))  *
             ((IP1[2] == IP2[2])  * (IP1[3] == IP2[3]))) *
             (((IP1[4] == IP2[4]) * (IP1[5] == IP2[5]))  *
             ((IP1[6] == IP2[6])  * (IP1[7] == IP2[7]))) ;
    answer.write("c");

    /* Export to file the "tracked" circuit */
    CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "ipv6");

}
