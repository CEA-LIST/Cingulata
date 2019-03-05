#include <cassert>

#include <int_op_gen/impl/operator.hxx>

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

CiBit CompOper::operator()(const CiBitVector& lhs, const CiBitVector& rhs) const {
  assert(lhs.size() == rhs.size());
  assert(lhs.size() > 0);

  return oper(lhs, rhs);
}

CiBitVector MuxOper::operator()(const CiBitVector& cond, const std::vector<CiBitVector>& inps) const {
  /* all inputs has same bit-size */
  auto s = inps[0].size();
  for (const auto& inp: inps)
    assert(inp.size() == s);

  assert((1U<<cond.size()) == inps.size());

  return oper(cond, inps);
}
