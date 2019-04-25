#include <int_op_gen/impl/multi_inp_adder.hxx>

#include <cmath>
#include <iostream>
#include <queue>
#include <tuple>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

MultiInputAdder::MultiInputAdder(const std::function<BinaryOper::signature> &p_bin_adder)
    : bin_adder(p_bin_adder) {}

CiBitVector MultiInputAdder::oper(const std::vector<CiBitVector> &inps) const {
  /* Determine the maximum bit-size of the results \sum_i (2^inps[i].size()-1) */
  vector<CiBitVector> inps_sym;
  for (const CiBitVector& inp: inps) {
    CiBitVector inp_sym(inp.size(), 1);
    // for (const CiBit& b: inp)

    inps_sym.push_back(inp_sym); // encode maximal inp.size()-bit value
  }

  CiBitVector sum_sym = add(inps_sym);

  /* first set bit, starting from MSB, gives output size */
  int out_size = sum_sym.size();
  while (sum_sym[out_size-1].get_val() == 0 and out_size > 0) out_size--;

  /* actual computation */
  return add(inps, out_size);
}

CiBitVector MultiInputAdder::add(const std::vector<CiBitVector> &inps, const unsigned max_size) const {
  using T = tuple<int, CiBitVector>;

  priority_queue<T, vector<T>, function<bool(const T &, const T &)>>
  elems_sorted_by_size(
      [](const T &a, const T &b) -> bool { return get<0>(a) > get<0>(b); });

  for (const CiBitVector& inp: inps)
    elems_sorted_by_size.push(forward_as_tuple(inp.size(), inp));

  while (elems_sorted_by_size.size() > 1) {
    int sa, sb;
    CiBitVector a, b;

    tie(sa, a) = elems_sorted_by_size.top();
    elems_sorted_by_size.pop();
    tie(sb, b) = elems_sorted_by_size.top();
    elems_sorted_by_size.pop();

    unsigned sr = (sa > sb ? sa : sb) + 1;
    sr = sr > max_size ? max_size : sr;
    a.resize(sr);
    b.resize(sr);

    CiBitVector r = bin_adder(a,b);

    elems_sorted_by_size.push(forward_as_tuple(r.size(), r));
  }

  return get<1>(elems_sorted_by_size.top());
}

