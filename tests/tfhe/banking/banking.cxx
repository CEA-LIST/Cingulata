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
#include <ci_fncs.hxx>
#include <ci_int.hxx>
#include <int_op_gen/size.hxx>
#include <tfhe_bit_exec.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Set context to TFHE bit tracker and size minimized integer operations */
  CiContext::set_config(
      make_shared<TfheBitExec>("tfhe.pk", TfheBitExec::Public),
      make_shared<IntOpGenSize>());

  CiInt t1{CiInt::s32}, t2{CiInt::s32}, m{CiInt::s32};
  t1.read("t1");
  t2.read("t2");
  m.read("m");

  CiInt t1_new = t1.resize(33) - m; // additional 1 bit for the overflow
  CiInt t2_new = t2 + m;
  CiBit cn = t1_new.msb();

  t1 = select(cn, t1, t1_new.resize(32));
  t2 = select(cn, t2, t2_new);

  t1.write("t1");
  t2.write("t2");
}
