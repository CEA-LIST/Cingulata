/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

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

#include <bit_exec/tracker.hxx>
#include <ci_bit.hxx>
#include <ci_context.hxx>

using namespace cingulata;

int main() {
  CiContext::set_bit_exec(std::make_shared<BitTracker>());

  CiBit a;
  a.encrypt();

  CiBit d;
  d.encrypt();
  d = 1 ^ a;

  d.write();

  CiContext::get_bit_exec_t<BitTracker>()->export_blif(std::cout, "lala");
}
