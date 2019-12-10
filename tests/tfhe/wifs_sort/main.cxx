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
#include <bit_exec/decorator/attach.hxx>
#include <bit_exec/decorator/stat.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <ci_fncs.hxx>
#include <int_op_gen/size.hxx>
#include <tfhe_bit_exec.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Set context to tfhe bit executor and size minimized integer
   * operations */
  CiContext::set_config(
      make_shared<decorator::Attach<TfheBitExec, decorator::Stat<IBitExecFHE>>>(
          "tfhe.pk", TfheBitExec::Public),
      make_shared<IntOpGenSize>());

  vector<CiInt> a(N, CiInt::u8);
  for (int i = 0; i < N; ++i)
    a[i].read("a_" + to_string(i));

  for (int i = 0; i < N-1; ++i) {
    for (int j = i+1; j < N; ++j) {
      CiBit swap = a[i] > a[j];
      CiInt t = select(swap, a[i], a[j]);
      a[i] = select(swap, a[j], a[i]);
      a[j] = t;
    }
  }

  // for (int i = 0; i < N-1; ++i) {
  //   for (int j = 1; j < N-i; ++j) {
  //     CiBit swap = a[j-1] > a[j];
  //     CiInt t = select(swap, a[j-1], a[j]);
  //     a[j-1] = select(swap, a[j], a[j-1]);
  //     a[j] = t;
  //   }
  // }

  for (int i = 0; i < N; ++i)
    a[i].write("r_" + to_string(i));

  CiContext::get_bit_exec_t<decorator::Stat<IBitExecFHE>>()->print();
}
