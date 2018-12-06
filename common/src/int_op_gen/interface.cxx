#include <int_op_gen/interface.hxx>

using namespace cingulata;

CiBit IIntOpGen::equal(const BitVectorT& lhs, const BitVectorT& rhs) const {
  const int size = lhs.size();

  CiBit equal(1);
  for (int i = 0; i < size; ++i) {
    equal &= lhs[i] == rhs[i];
  }

  return equal;
}

CiBit IIntOpGen::not_equal(const BitVectorT& lhs, const BitVectorT& rhs) const {
  return !equal(lhs,rhs);
}

CiBit IIntOpGen::lower(const BitVectorT& lhs, const BitVectorT& rhs, const bool signed_comp) const {
  return compare(lhs, rhs, 0, signed_comp);
}
CiBit IIntOpGen::greater(const BitVectorT& lhs, const BitVectorT& rhs, const bool signed_comp) const {
  return compare(rhs, lhs, 0, signed_comp);
}
CiBit IIntOpGen::lower_equal(const BitVectorT& lhs, const BitVectorT& rhs, const bool signed_comp) const {
  return compare(lhs, rhs, 1, signed_comp);
}
CiBit IIntOpGen::greater_equal(const BitVectorT& lhs, const BitVectorT& rhs, const bool signed_comp) const {
  return compare(rhs, lhs, 1, signed_comp);
}
