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

/**
 * IPv6 equality test, each IPv6 is represented by eight Integer16.
 **/

/* compiler includes */
#include <fstream>
#include <iostream>

/* local includes */
#include <integer.hxx>

/* namespaces */
using namespace std;

int main() {

/** IPv6 can be splitted into 8 integers of 2 octets. **/
  Integer16 IP1[8]; 
  Integer16 IP2[8]; 

  for (int i = 0; i < 8; i++) {
    cin >> IP1[i];
  }
  for (int i = 0; i < 8; i++) {
    cin >> IP2[i];
  }

/** equality test **/
  cout << (((IP1[0] == IP2[0]) * (IP1[1] == IP2[1])) *
           ((IP1[2] == IP2[2]) * (IP1[3] == IP2[3]))) *
          (((IP1[4] == IP2[4]) * (IP1[5] == IP2[5])) *
           ((IP1[6] == IP2[6]) * (IP1[7] == IP2[7])));


  FINALIZE_CIRCUIT(blif_name);

}
