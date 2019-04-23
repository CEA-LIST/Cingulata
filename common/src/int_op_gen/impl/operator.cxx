#include <cassert>

#include <int_op_gen/impl/operator.hxx>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector UnaryOper::operator()(const CiBitVector& rhs) const {
  assert(rhs.size() > 0);

  return oper(rhs);
}

CiBitVector BinaryOper::operator()(const CiBitVector& lhs, const CiBitVector& rhs) const {
  assert(lhs.size() == rhs.size());
  assert(lhs.size() > 0);

  return oper(lhs, rhs);
}

CiBitVector NaryOper::operator()(const vector<CiBitVector>& inps) const {
  if (inps.size() == 0) {
    return CiBitVector();
  } else if (inps.size() == 1) {
    return inps[0];
  } else {
    return oper(inps);
  }
}

CiBit CompOper::operator()(const CiBitVector& lhs, const CiBitVector& rhs) const {
  assert(lhs.size() == rhs.size());
  assert(lhs.size() > 0);

  return oper(lhs, rhs);
}

CiBitVector MuxOper::operator()(const CiBitVector& cond, const vector<CiBitVector>& p_inps) const {
  /* resize inputs to same bit-size */
  vector<CiBitVector> inps = p_inps;

  int max_size = 0;
  for (const auto& inp: inps)
    if (max_size < (int)inp.size())
      max_size = inp.size();

  if (max_size == 0)
    return CiBitVector();

  for (auto& inp: inps)
    inp.resize(max_size);

  assert((1U<<cond.size()) == inps.size());

  return oper(cond, inps);
}
