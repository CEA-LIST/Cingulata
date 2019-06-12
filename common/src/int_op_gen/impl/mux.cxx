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

#include <int_op_gen/impl/mux.hxx>
#include <int_op_gen/impl/dec.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;
using namespace std;

CiBitVector MuxDepth::oper(const CiBitVector& cond, const vector<CiBitVector>& inps) const {
  const static Decoder decoder;
  const unsigned n = inps[0].size();

  CiBitVector dec_cond = decoder(cond);

  CiBitVector res(n, 0);
  for (unsigned i = 0; i < dec_cond.size(); ++i) {
    res ^= inps[i] & CiBitVector(n, dec_cond[i]);
  }

  return res;
}
