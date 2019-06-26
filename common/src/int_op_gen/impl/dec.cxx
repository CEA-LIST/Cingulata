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

#include <int_op_gen/impl/dec.hxx>

#include <vector>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector Decoder::oper(const CiBitVector& c) const {
  const int n = c.size();
  CiBitVector res;

  if (n == 1) {
    res = vector<CiBit>({
      op_not  (c[0]),
               c[0]
    });
  }
  else if (n == 2) {
    res = vector<CiBit>({
      op_nor    (c[0], c[1]),
      op_andyn  (c[0], c[1]),
      op_andny  (c[0], c[1]),
      op_and    (c[0], c[1])
    });
  }
  else {
    const int n2 = n>>1;

    CiBitVector cl = oper(c.slice(0, n2));
    CiBitVector ch = oper(c.slice(n2, n));

    for (unsigned i = 0; i < ch.size(); ++i)
      for (unsigned j = 0; j < cl.size(); ++j)
        res.append(op_and(ch[i], cl[j]));
  }

  return res;
}
