#include "int_op_gen/mult_depth.hxx"

using namespace cingulata;

CiBit IntOpGenDepth::not_equal(const BitVectorT& lhs, const BitVectorT& rhs) const {
  const int size = lhs.size();

  BitVectorT tmp(size, 0);
  for (int i = 0; i < size; ++i)
    tmp[i] = lhs[i] != rhs[i];

  /* log depth tree */
  for (int k = 1; k < size; k *= 2) {
    for (int i = 0; i < size - k; i += 2*k) {
      tmp.at(i) &= tmp.at(i+k);
    }
  }

  return tmp[0];
}
