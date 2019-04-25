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


std::vector<CiBitVector> SortOper::operator()(const std::vector<CiBitVector>& v_cbv,
                                              const std::vector<CiBitVector>& i_cbv,
                                              const bool reverse) const {
  auto s = v_cbv[0].size();
  assert(v_cbv.size() == i_cbv.size());
  for (const auto& cbv: v_cbv)
    assert(cbv.size() == s);
  for (const auto& cbv: i_cbv)
    assert(cbv.size() == s);
  return oper(v_cbv, i_cbv, reverse);
}

std::vector<CiBitVector> SortOper::operator()(const std::vector<CiBitVector>& v_cbv,
                                              const bool reverse) const {
  return (*this)(v_cbv, v_cbv, reverse);
}