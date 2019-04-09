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
#include <iostream>
#include <fstream>

/* local includes */
#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <ci_bit.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

#define OUT_BITCNT 8 //depth 12

void readKeyAndRunTrivium(void)
{
	CiBit key[80], iv[80], s[288];

	for(int i = 0; i < 80; i++) {
		key[i].read("k" + to_string(i));
		iv[i].read("v" + to_string(i));
  }

	// Internal state setup.
	for(int i = 0; i < 288; i++)
		s[i] = false;
	for(int i = 0; i < 80; i++) {
	// Insert the key @ position 0 (1 in the paper).
		s[i] = key[i];
	// Insert the iv @ position 93 (94 in the paper).
		s[i+93] = iv[i];
  }
	// Set pos 285, 286 and 287 to 1 (286, 287 and 288 in the paper).
	s[285] = true;
	s[286] = true;
	s[287] = true;

	// Internal state warm up.
	for(int i=0; i < 4*288; i++)
	{
		CiBit t1 = s[65]^(s[90]&s[91])^s[92]^s[170]; // t1 = s66+s91*s92+s93+s171 (paper).
		CiBit t2 = s[161]^(s[174]&s[175])^s[176]^s[263]; // t2 = s162+s175*s176+s177+s264 (paper).
		CiBit t3 = s[242]^(s[285]&s[286])^s[287]^s[68]; // t3 = s243+s286*s287+s288+s69 (paper).

    // Shift the internal state from 1 to the right.
		for(int j = 0; j < 288; j++)
			s[288-j-1] = s[288-j-2];

    // Then inject the ti's.
		s[0] = t3; // s1 = t3 (paper).
		s[93] = t1; // s94 = t1 (paper).
		s[177] = t2; // s178 = t2 (paper).
	}

	// Keystream generation.
	for(int i = 0; i < OUT_BITCNT; i++)
	{
		CiBit t1 = s[65]^s[92]; // t1 = s66+s93 (paper).
		CiBit t2 = s[161]^s[176]; // t2 = s162+s177 (paper).
		CiBit t3 = s[242]^s[287]; // t3 = s243+s288 (paper).

		CiBit z = t1^t2^t3; // zi = t1+t2+t3 (paper).

    t1 = t1^(s[90]&s[91])^s[170]; // t1 = t1+s91*s92+s171 (paper).
		t2 = t2^(s[174]&s[175])^s[263]; // t2 = t2+s175*s176+s264 (paper).
		t3 = t3^(s[285]&s[286])^s[68]; // t3 = t3+s286*s287+s69 (paper).

    // Shift the internal state from 1 to the right.
		for(int j = 0; j < 288; j++)
			s[288-j-1] = s[288-j-2];
		// Then inject the ti's.
		s[0] = t3; // s1 = t3 (paper).
		s[93] = t1; // s94 = t1 (paper).
		s[177] = t2; // s178 = t2 (paper).

		//cout<<"z["<<i<<"] = " << z <<endl;
		z.write();
	}
}

int main() {
  CiContext::set_config(new BitTracker(), new IntOpGenDepth());

  readKeyAndRunTrivium();

  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "trivium");
}

