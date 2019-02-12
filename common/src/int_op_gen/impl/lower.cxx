#include <int_op_gen/impl/lower.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBit LowerCompDepth::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int len = lhs.size();
  if (len == 1) return CiBit(lhs[0]).op_andny(rhs[0]);

  const int len2 = len>>1;
  CiBitVector lhs_l = lhs.slice(0,len2);
  CiBitVector lhs_h = lhs.slice(len2);
  CiBitVector rhs_l = rhs.slice(0,len2);
  CiBitVector rhs_h = rhs.slice(len2);

  return oper(lhs_h, rhs_h) ^ (equal(lhs_h, rhs_h) & oper(lhs_l, rhs_l));
}


CiBit LowerCompSize::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit carry{0};
  for (int i = 0; i < size; ++i) {
    CiBit n1 = carry ^ lhs[i];
    CiBit n2 = carry ^ rhs[i];
    carry = (n1 & n2) ^ rhs[i];
  }
  return carry;
}
