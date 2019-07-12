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
#include <bit_exec/decorator/attach.hxx>
#include <bit_exec/decorator/stat.hxx>
#include <ci_context.hxx>
#include <ci_fncs.hxx>
#include <ci_int.hxx>
#include <int_op_gen/size.hxx>
#include <tfhe_bit_exec.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Set context to tfhe bit executor and size minimized integer
   * operations */
  CiContext::set_config(
      make_shared<decorator::Attach<TfheBitExec, decorator::Stat<TfheBitExec>>>(
          "tfhe.sk", TfheBitExec::Secret),
      make_shared<IntOpGenSize>());

  CiInt t{CiInt::s32}, m0{CiInt::s32}, nd0{CiInt::s32};
  t.read("t");
  m0.read("m");
  nd0.read("nd");

  uint8_t d = 2; // number of days from previous transaction

  CiInt m1p = m0 + t; // potential new balance
  CiBit m0_neg = m0.sign();
  CiBit m1_neg = m1p.sign();

  // if previous and current amounts are negative then subtract d days from negative days counter
  CiInt nd1 = select(m0_neg && m1_neg, nd0 - d, 0);
  CiBit nd1_neg = nd1.sign();

  CiBit m1_under_min = m1p < CiInt::s32v(-500);
  CiBit nd1_under_min = nd1 < CiInt::s32v(-7);

  /* when balance and negative days counter is positive (i.e. negative days
   * counter equals to zero) => transaction ok */
  CiBit status_ok = !(m1_neg || nd1_neg);

  /* if either balance or negative days counter are under minimal value => transaction error */
  CiBit status_error = m1_under_min || nd1_under_min;

  /* if neither status is ok nor error => transaction warn */
  CiBit status_warn = !(status_ok || status_error);

  /* update new account balance only if no error */
  CiInt m1 = select(status_error, m0, m1p);

  m1.write("m");
  nd1.write("nd");
  status_ok.write("status_ok");
  status_warn.write("status_warn");
  status_error.write("status_error");

  CiContext::get_bit_exec_t<decorator::Stat<TfheBitExec>>()->print();
}
