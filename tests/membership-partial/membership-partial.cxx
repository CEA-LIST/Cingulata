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

  
  vector<CiInt> IP_target(4, CiInt::u8);
  vector < vector  <CiInt> >  IP        (list_size, vector<CiInt>(4,CiInt::u8));  

  for (int i=0; i<4; i++)
  {
  
      cin >> IP_target[i];
      //IP_target[i].read("a_"+to_string(i));
  }

  for (int i = 0; i < list_size; i++)
  {
      for (int j=0; j<4; j++)
       {
           
           // b_"i"_"j"_"bit number 0 7"
           IP[i][j].read("b_"+to_string(i)+"_"+to_string(j));
           //cin >> IP[i][j];
           
       }
  }

  // Question: Does IP_target belongs to IP table?
  // Prerequisite : IP table contains distinct IPs (needed if we use xor to
  // compute answer below, it is better to do so else, if we use or, then
  // multiplicative depth increases)

 
  


  CiBit answer(0);

  for (int i = 0; i < list_size; i++) {
    answer = answer xor (((IP_target[0] == IP[i][0])*(IP_target[1] == IP[i][1]))*((IP_target[2] == IP[i][2])*(IP_target[3] == IP[i][3]))); // list with no duplicate
  }

  answer.write("s");
  
  /* Export to file the "tracked" circuit */
  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "membership-partial");


}
