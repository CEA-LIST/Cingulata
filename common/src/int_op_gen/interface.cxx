#include <int_op_gen/interface.hxx>

using namespace cingulata;

CiBit IIntOpGen::equal(const BitVectorT& lhs, const BitVectorT& rhs) const {
  return !not_equal(lhs,rhs);
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
