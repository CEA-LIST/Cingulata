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
      elems_sorted_by_depth.push(forward_as_tuple(1, (rhs >> i) & lhs[i]));

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
