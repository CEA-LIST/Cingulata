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

#include <cstdint>

/* local includes */
#include <bit_exec/decorator/attach.hxx>
#include <bit_exec/decorator/depth.hxx>
#include <bit_exec/decorator/stat.hxx>
#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

int main() {
  /* Set context to bit tracker and multiplicative depth minimized integer
   * operations */
  CiContext::set_config(
      make_shared<
          decorator::Attach<BitTracker, decorator::Depth, decorator::Stat>>(),
      make_shared<IntOpGenDepth>());

  CiInt a{CiInt::u8};      // create from unsigned 8-bit template
  CiInt b{CiInt::u8v(42)}; // use helper function to create an unsigned 8-bit
  CiInt c{-1, 16, false};  // or manually specify value, size and signedness

  a.read("a"); // read variable a and set name

  b.set_name("b"); // set name and ...
  cin >> b;        //  read variable b

  c = a + b;

  cout << c.set_name("c"); // set name and write variable
  // c.write("c");           //  or do it at once

  /* Export to file the "tracked" circuit */
  CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "hello");

  CiContext::get_bit_exec_t<decorator::Depth<BitTracker>>()->print();
  CiContext::get_bit_exec_t<decorator::Stat<BitTracker>>()->print();
}
