#include <int_op_gen/mult_depth.hxx>

using namespace cingulata;

CiBit IntOpGenDepth::not_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBitVector tmp(size, 0);
  for (int i = 0; i < size; ++i)
    tmp[i] = lhs[i] != rhs[i];

  /* log depth tree */
  for (int k = 1; k < size; k *= 2) {
    for (int i = 0; i < size - k; i += 2*k) {
      tmp[i] &= tmp[i+k];
    }
  }

  return tmp[0];
}

CiBit IntOpGenDepth::lower(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  assert(lhs.size() == rhs.size());

  const auto len = lhs.size();
  if (len == 1) return CiBit(lhs[0]).op_andny(rhs[0]);

  const auto len2 = len>>1;
  CiBitVector lhs_l = lhs.slice(0,len2);
  CiBitVector lhs_h = lhs.slice(len2);
  CiBitVector rhs_l = rhs.slice(0,len2);
  CiBitVector rhs_h = rhs.slice(len2);

  return lower(lhs_h, rhs_h) ^ (equal(lhs_h, rhs_h) & lower(lhs_l, rhs_l));
}

