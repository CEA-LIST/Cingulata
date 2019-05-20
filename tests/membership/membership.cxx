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

    CiContext::set_config(make_shared<BitTracker>(), make_shared<IntOpGenDepth>());

  CiInt IP_target{CiInt::s32};
  vector<CiInt> IP(list_size, CiInt::s32);
  
  IP_target.read("a");

  for (int i = 0; i < list_size; i++) {
	IP[i].read("b_"+to_string(i));
  }
  
  CiBit  answer(0); 

  for (int i = 0; i < list_size; i++) {
    answer = answer xor (IP_target == IP[i]); // list with no duplicate
  }
  
  answer.write("s");
  
   /* Export to file the "tracked" circuit */
  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "membership");
}
