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
    CiBitVector inp_sym(inp.size(), 1); // encode maximal inp.size()-bit value

    /* set plaintext bits if available */
    for (unsigned i = 0; i < inp.size(); ++i) {
      if (inp[i].is_plain() and inp[i].get_val() == 0)
        inp_sym[i] = 0;
    }

    inps_sym.push_back(inp_sym);
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
  elems_sorted_by_depth(
      [](const T &a, const T &b) -> bool { return get<0>(a) > get<0>(b); });

  for (const CiBitVector& inp: inps)
    elems_sorted_by_depth.push(forward_as_tuple(1, inp));

  while (elems_sorted_by_depth.size() > 2) {
    int sa, sb, sc;
    int da, db, dc;
    CiBitVector a, b, c;

    tie(da, a) = elems_sorted_by_depth.top();
    elems_sorted_by_depth.pop();
    tie(db, b) = elems_sorted_by_depth.top();
    elems_sorted_by_depth.pop();
    tie(dc, c) = elems_sorted_by_depth.top();
    elems_sorted_by_depth.pop();
    sa=a.size();
    sb=b.size();
    sc=c.size();

    int stmp = (sa > sb ? sa : sb);
    unsigned sr = (sc > stmp ? sc : stmp);
    sr = sr > max_size ? max_size : sr;
    CiBitVector tmp1 = a ^ b ^ c;
    /* equalize sizes */
    a.resize(sr);
    b.resize(sr);
    c.resize(sr);
    /* increase size if necessary */
    if (sr < max_size)
      if ( a[sr-1].get_val()==1 or b[sr-1].get_val()==1 or c[sr-1].get_val()==1){
        sr+=1;
        a.resize(sr);
        b.resize(sr);
        c.resize(sr);
      }
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

  unsigned sr = (a.size() > b.size() ? a.size() : b.size());
  /* equalize sizes */
  a.resize(sr);
  b.resize(sr);
  if (sr < max_size)
    if ( a[sr-1].get_val()==1 or b[sr-1].get_val()==1){
      sr+=1;
      a.resize(sr);
      b.resize(sr);
    }
  return bin_adder(a,b);
}

