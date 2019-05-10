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
#include <int_op_gen/size.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Set context to bit tracker and multiplicative depth minimized integer
   * operations */
  CiContext::set_config(make_shared<TfheBitExec>("tfhe.pk", TfheBitExec::Public), make_shared<IntOpGenSize>());

  CiInt a{CiInt::u8};     // create from unsigned 8-bit template
  CiInt b{0, 8, false};   // manually specify value, size and signedness
  CiInt c{(uint16_t)-1};  // automatically determine size and signedness from value

  a.read("a");
  b.read("b");

  c = a + b;
  // c = a * a * b - a;

  c.write("c");
}
