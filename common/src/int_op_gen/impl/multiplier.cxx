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

#include <int_op_gen/impl/adder.hxx>
#include <int_op_gen/impl/multiplier.hxx>

#include <queue>
#include <tuple>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector WallaceMultiplier::oper(const CiBitVector &lhs,
                                    const CiBitVector &rhs) const {
  if (lhs.size() == 1) {
    CiBitVector res = lhs & rhs;
    return res;
  } else {
    using T = tuple<int, CiBitVector>;
    priority_queue<T, vector<T>, function<bool(const T &, const T &)>>
    elems_sorted_by_depth(
        [](const T &a, const T &b) -> bool { return get<0>(a) > get<0>(b); });

    for (unsigned int i = 0; i < lhs.size(); ++i)
      elems_sorted_by_depth.push(forward_as_tuple(1, (rhs >> i) & CiBitVector(rhs.size(), lhs[i])));

    while (elems_sorted_by_depth.size() > 2) {
      int da, db, dc;
      CiBitVector a, b, c;

      tie(da, a) = elems_sorted_by_depth.top();
      elems_sorted_by_depth.pop();
      tie(db, b) = elems_sorted_by_depth.top();
      elems_sorted_by_depth.pop();
      tie(dc, c) = elems_sorted_by_depth.top();
      elems_sorted_by_depth.pop();

      CiBitVector tmp1 = a ^ b ^ c;
      a >>= 1;
      b >>= 1;
      c >>= 1;
      CiBitVector tmp2 = ((a ^ c) & (b ^ c)) ^ c;

      elems_sorted_by_depth.push(forward_as_tuple(dc, tmp1));
      elems_sorted_by_depth.push(forward_as_tuple(dc + 1, tmp2));
    }

    int da, db;
    CiBitVector a, b;

    tie(da, a) = elems_sorted_by_depth.top();
    elems_sorted_by_depth.pop();
    tie(db, b) = elems_sorted_by_depth.top();
    elems_sorted_by_depth.pop();

    return adder(a, b);
  }
}
