#include "int_op_gen/mult_depth.hxx"

using namespace cingulata;

CiBit IntOpGenDepth::compare(const BitVectorT& lhs, const BitVectorT& rhs,
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
