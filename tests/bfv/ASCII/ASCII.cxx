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

/* local includes */
#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  CiContext::set_config(make_shared<BitTracker>(),
                        make_shared<IntOpGenDepth>());

  // The inputs are encrypted decimal encodings of ASCII characters of target
  // and then of each string in the list The output encrypted bit indicates if
  // target belongs to the list.
  vector<CiInt> ASCII_target(nb_chars, CiInt::u8);
  vector<vector<CiInt>> ASCII_list(nb_chars,
                                   vector<CiInt>(list_size, CiInt::u8));

  for (int i = 0; i < nb_chars; i++) {
    cin >> ASCII_target[i];
  }

  for (int j = 0; j < list_size; j++) {
    for (int i = 0; i < nb_chars; i++) {
      cin >> ASCII_list[i][j];
    }
  }
  CiBit answer(0);

  CiBit is_equal[list_size];
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

  answer.write("s");

  /* Export to file the "tracked" circuit */
  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "ASCII");
}
