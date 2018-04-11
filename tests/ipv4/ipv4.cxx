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
 * IPv4 equality test, each IPv4 is represented by one Integer32.
 **/

/* compiler includes */
#include <fstream>
#include <iostream>

/* local includes */
#include <integer.hxx>

/* namespaces */
using namespace std;

int main() {

  Integer32 IP1, IP2;
  cin >> IP1;
  cin >> IP2;
  
  // equality test
  cout << (IP1 == IP2);

  FINALIZE_CIRCUIT(blif_name);

}
