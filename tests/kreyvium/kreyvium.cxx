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
#include <ci_bit_vector.hxx>
#include <ci_context.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

#define OUT_BITCNT 46 // depth 12

void kreyvium_warm(CiBitVector &state, CiBitVector &RKEY, CiBitVector &RIV,
                   const CiBitVector &key, const CiBitVector &iv) {
  RKEY.resize(128);
  RIV.resize(128);

  // Internal state setup.
  state.resize(288, CiBit::one);

  // Insert the first 93 key bits @ position 0 (1 in the paper).
  state.slice() = key.slice(0, 93);

  // Insert the first iv bits @ position 93 (94 in the paper).
  state.slice(93) = iv;

  // Set last pos (287) to 0 (288 in the paper).
  state[-1] = 0;

  // RKEY register set up.
  RKEY.slice() = key.slice({}, {}, -1);

  // RIV register setup.
  RIV.slice() = iv.slice({}, {}, -1);

  // Internal state warm up.
  for (int i = 0; i < 4 * 288; i++) {
    CiBit t1 = state[65] + state[90] * state[91] + state[92] + state[170] +
               RIV[-1]; // t1 = s66+s91*s92+s93+s171+RIV[0] (paper).
    CiBit t2 = state[161] + state[174] * state[175] + state[176] +
               state[263]; // t2 = s162+s175*s176+s177+s264 (paper).
    CiBit t3 = state[242] + state[285] * state[286] + state[287] + state[68] +
               RKEY[-1]; // t3 = s243+s286*s287+s288+s69+RKEY[0] (paper).

    // Shift the internal state from 1 to the right.
    state.ror(1);

    // Then inject the ti's.
    state[0] = t3;   // s1 = t3 (paper).
    state[93] = t1;  // s94 = t1 (paper).
    state[177] = t2; // s178 = t2 (paper).

    // Shift RKEY and RIV from 1 to the right.
    RKEY.ror(1);
    RIV.ror(1);
  }
}

CiBitVector kreyvium_stream(CiBitVector &state, CiBitVector &RKEY,
                            CiBitVector &RIV, const int nb_bits) {
  CiBitVector bs(nb_bits);

  // Keystream generation.
  for (int i = 0; i < nb_bits; i++) {
    CiBit t1 = state[65] + state[92];   // t1 = s66+s93 (paper).
    CiBit t2 = state[161] + state[176]; // t2 = s162+s177 (paper).
    CiBit t3 =
        state[242] + state[287] + RKEY[-1]; // t3 = s243+s288+RKEY[0] (paper).

    bs[i] = t1 + t2 + t3; // zi = t1+t2+t3 (paper).

    t1 = t1 + state[90] * state[91] + state[170] +
         RIV[-1]; // t1 = t1+s91*s92+s171 (paper).
    t2 = t2 + state[174] * state[175] +
         state[263]; // t2 = t2+s175*s176+s264 (paper).
    t3 = t3 + state[285] * state[286] +
         state[68]; // t3 = t3+s286*s287+s69 (paper).

    // Shift the internal state from 1 to the right.
    state.ror(1);

    // Then inject the ti's.
    state[0] = t3;   // s1 = t3 (paper).
    state[93] = t1;  // s94 = t1 (paper).
    state[177] = t2; // s178 = t2 (paper).

    // Shift RKEY and RIV from 1 to the right.
    RKEY.ror(1);
    RIV.ror(1);
  }

  return bs;
}

int main() {
  CiContext::set_bit_exec(new BitTracker());

  CiBitVector key{128}, iv{128};

  key.read("key");
  iv.read("iv");

  CiBitVector state, RKEY, RIV;
  kreyvium_warm(state, RKEY, RIV, key, iv);

  CiBitVector bs = kreyvium_stream(state, RKEY, RIV, OUT_BITCNT);
  bs.write("out");

  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "kreyvium");
}
