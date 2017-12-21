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
 * IPv4 list intersection
 **/

/* compiler includes */
#include <fstream>
#include <iostream>

/* local includes */
#include <integer.hxx>

/* namespaces */
using namespace std;

int main() {
  Integer32 first_list[first_list_size], second_list[second_list_size];

  for (int i = 0; i < first_list_size; i++) {
    cin >> first_list[i];
  }

  for (int i = 0; i < second_list_size; i++) {
    cin >> second_list[i];
  }

  // Question: What is the intersection of the two tables?

  ////Convention: if two IPs are différents, the program returns the integer32
  /// 0, the circuit returns 32 zero bits , the shell script does not print
  ////Prerequisite: IP tables does not contain 0.0.0.0


  /** Method 2: Membership tests + Conditional expressions **/
  // Method 2 is favoured since circuits contains less multiplicative gates
  // (first_list_size select with method 2 against
  // first_list_size*second_list_size select with method 1).
  // Prerequisite 2: IP table contains distinct IPs (needed if we use xor to
  // compute answer below, it is better to do so. Else, if we use "or", then
  // multiplicative depth increases)

  Bit are_equal[first_list_size] = {};
  for (int i = 0; i < first_list_size; i++) {
    for (int j = 0; j < second_list_size; j++) {
      are_equal[i] = are_equal[i] xor
                     (first_list[i] == second_list[j]); // lists with no duplicate
    }
  }
  Integer32 not_member(0);
  for (int i = 0; i < first_list_size; i++) {
    cout << select(are_equal[i], first_list[i], not_member);
  }

	FINALIZE_CIRCUIT("intersection.blif");

}
