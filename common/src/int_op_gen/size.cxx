#include "int_op_gen/size.hxx"

using namespace cingulata;

CiBit IIntOpGenSize::not_equal(const BitVectorT& lhs, const BitVectorT& rhs) const {
  const int size = lhs.size();

  CiBit ne(1);
  for (int i = 0; i < size; ++i) {
    ne &= lhs[i] != rhs[i];
  }

  return ne;
}

CiBit IIntOpGenSize::compare(const BitVectorT& lhs, const BitVectorT& rhs,
    const int carry_inp, const bool signed_comp) const
{
  const int size = lhs.size();

  CiBit carry(carry_inp);
  CiBit n1, n2;
  for (int i = 0; i < size; ++i) {
    n1 = carry ^ lhs.at(i);
    n2 = carry ^ rhs.at(i);
    carry = n1 & n2;
    carry ^= rhs.at(i);
  }
  if (signed_comp)
    carry ^= n1 ^ n2;

  return carry;
}
