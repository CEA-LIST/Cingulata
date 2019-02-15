#include <int_op_gen/impl/equal.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBit EqualDepth::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBitVector tmp(size, 0);
  for (int i = 0; i < size; ++i)
    tmp[i] = lhs[i] == rhs[i];

  /* log depth tree */
  for (int k = 1; k < size; k *= 2) {
    for (int i = 0; i < size - k; i += 2*k) {
      tmp[i] &= tmp[i+k];
    }
  }

  return tmp[0];
}

CiBit EqualSize::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit res{1};
  for (int i = 0; i < size; ++i) {
    res &= lhs[i] == rhs[i];
  }

  return res;
}
