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

#include <iostream>

/* local includes */
#include <tfhe_bit_exec.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>


/* namespaces */
using namespace std;
using namespace cingulata;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <a> <b>" << endl;
    exit(-1);
  }

  /* get input values */
  int a_val = atoi(argv[1]);
  int b_val = atoi(argv[2]);

  /* Only tfhe bit executor is needed for encryption/decryption and IO operations  */
  CiContext::set_bit_exec(new TfheBitExec("tfhe.sk", TfheBitExec::Secret));

  CiInt a{a_val, 8, false};
  CiInt b{b_val, 8, false};

  a.encrypt().write("a");
  b.encrypt().write("b");
}
