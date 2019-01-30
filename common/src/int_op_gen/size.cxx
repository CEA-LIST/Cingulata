#include "int_op_gen/size.hxx"

using namespace cingulata;

CiBit IntOpGenSize::not_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit ne(1);
  for (int i = 0; i < size; ++i) {
    ne &= lhs[i] != rhs[i];
  }

  return ne;
}

CiBit IntOpGenSize::lower(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit carry{0};
  CiBit n1, n2;
  for (int i = 0; i < size; ++i) {
    n1 = carry ^ lhs[i];
    n2 = carry ^ rhs[i];
    carry = n1 & n2;
    carry ^= rhs[i];
  }

  return carry;
}
