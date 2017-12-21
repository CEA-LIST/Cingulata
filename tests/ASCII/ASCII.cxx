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


/* compiler includes */
#include <fstream>
#include <iostream>

/* local includes */
#include <integer.hxx>

/* namespaces */
using namespace std;

int main() {
  Integer8 ASCII_target[nb_chars] = {},
           ASCII_list[nb_chars][list_size] = {}; // one ASCII string is
                                                 // represented by nb__chars
                                                 // Integer8 (bytes)

  // The inputs are encrypted decimal encodings of ASCII characters of target
  // and then of each string in the list The output encrypted bit indicates if
  // target belongs to the list.

  for (int i = 0; i < nb_chars; i++) {
    cin >> ASCII_target[i];
  }

  for (int j = 0; j < list_size; j++) {
    for (int i = 0; i < nb_chars; i++) {
      cin >> ASCII_list[i][j];
    }
  }

  Bit answer(0);
  Bit is_equal[list_size];
  for (int j = 0; j < list_size; j++) {
    is_equal[j] = 1; // we need to initialize with 1 since we multiply to know
                     // if two strings are equal and list_size cant be zero.
  }

  for (int j = 0; j < list_size; j++) {
    for (int i = 0; i < nb_chars; i++) {
      is_equal[j] = is_equal[j] and (ASCII_target[i] == ASCII_list[i][j]);
    }
    answer = answer xor is_equal[j]; // list with no duplicate
  }
  
  cout << answer;

  FINALIZE_CIRCUIT("ASCII.blif");
}
