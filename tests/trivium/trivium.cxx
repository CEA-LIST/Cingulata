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

/* compiler includes */
#include <fstream>
#include <iostream>

/* local includes */
#include <bit_exec/tracker.hxx>
#include <ci_bit.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

#define OUT_BITCNT 57 // depth 12

void trivium_warm(CiBitVector &state, const CiBitVector &key,
                  const CiBitVector &iv) {
  assert(key.size() == 80);
  assert(iv.size() == 80);

  // Internal state setup.
  state.resize(288, CiBit::zero);

  // Insert the key bits @ position 0 (1 in the paper).
  state.slice(0) = key;

  // Insert the iv bits @ position 93 (94 in the paper).
  state.slice(93) = iv;

  // Set pos 285, 286 and 287 to 1 (286, 287 and 288 in the paper).
  state.slice(285, 288) = CiBitVector(3, 1);

  // Internal state warm up.
  for (int i = 0; i < 4 * 288; i++) {
    CiBit t1 = state[65] ^ (state[90] & state[91]) ^ state[92] ^
               state[170]; // t1 = s66+s91*s92+s93+s171 (paper).
    CiBit t2 = state[161] ^ (state[174] & state[175]) ^ state[176] ^
               state[263]; // t2 = s162+s175*s176+s177+s264 (paper).
    CiBit t3 = state[242] ^ (state[285] & state[286]) ^ state[287] ^
               state[68]; // t3 = s243+s286*s287+s288+s69 (paper).

    // Shift the internal state from 1 to the right.
    state.ror(1);

    // Then inject the ti's.
    state[0] = t3;   // s1 = t3 (paper).
    state[93] = t1;  // s94 = t1 (paper).
    state[177] = t2; // s178 = t2 (paper).
  }
}

CiBitVector trivium_stream(CiBitVector &state, const int nb_bits) {
  CiBitVector bs(nb_bits);

  // Keystream generation.
  for (int i = 0; i < nb_bits; i++) {
    CiBit t1 = state[65] ^ state[92];   // t1 = s66+s93 (paper).
    CiBit t2 = state[161] ^ state[176]; // t2 = s162+s177 (paper).
    CiBit t3 = state[242] ^ state[287]; // t3 = s243+s288 (paper).

    bs[i] = t1 ^ t2 ^ t3; // zi = t1+t2+t3 (paper).

    t1 = t1 ^ (state[90] & state[91]) ^
         state[170]; // t1 = t1+s91*s92+s171 (paper).
    t2 = t2 ^ (state[174] & state[175]) ^
         state[263]; // t2 = t2+s175*s176+s264 (paper).
    t3 = t3 ^ (state[285] & state[286]) ^
         state[68]; // t3 = t3+s286*s287+s69 (paper).

    // Shift the internal state from 1 to the right.
    state.ror(1);

    // Then inject the ti's.
    state[0] = t3;   // s1 = t3 (paper).
    state[93] = t1;  // s94 = t1 (paper).
    state[177] = t2; // s178 = t2 (paper).
  }

  return bs;
}

int main() {
  CiContext::set_bit_exec(make_shared<BitTracker>());

  CiBitVector key{80}, iv{80};

  key.read("key");
  iv.read("iv");

  CiBitVector state;
  trivium_warm(state, key, iv);

  // state.write("s");

  CiBitVector bs = trivium_stream(state, OUT_BITCNT);
  bs.write("out");

  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "trivium");
}
