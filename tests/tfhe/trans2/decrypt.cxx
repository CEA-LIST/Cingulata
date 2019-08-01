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
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <tfhe_bit_exec.hxx>

#include <unistd.h> // file existence check

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Only tfhe bit executor is needed for encryption/decryption and IO
   * operations  */
  CiContext::set_bit_exec(
      make_shared<TfheBitExec>("tfhe.sk", TfheBitExec::Secret));

  CiInt t{CiInt::u32};
  t.read("t").decrypt();
  printf("Transaction amount: %-8d\n", (int32_t)t.get_val());

  CiInt m{CiInt::u32};
  m.read("m").decrypt();
  printf("Account balance: %-8d\n", (int32_t)m.get_val());

  CiInt nd{CiInt::u32};
  nd.read("nd").decrypt();
  printf("Negative days counter: %-8d\n", -(int32_t)nd.get_val());

  if (access("status_ok", F_OK) != -1 and access("status_warn", F_OK) != -1 and
      access("status_error", F_OK) != -1) {
    CiBit tmp;

    int status_ok = tmp.read("status_ok").decrypt().get_val();
    int status_warn = tmp.read("status_warn").decrypt().get_val();
    int status_error = tmp.read("status_error").decrypt().get_val();

    assert(status_ok + status_warn + status_error == 1);

    if (status_ok) {
      printf("Status: OK\n");
    } else if (status_warn) {
      printf("Status: WARN\n");
    } else {
      printf("Status: ERROR\n");
    }
  }
}
