#include <int_op_gen/interface.hxx>

using namespace cingulata;

CiBit IIntOpGen::equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !not_equal(lhs,rhs);
}

CiBit IIntOpGen::greater(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return lower(rhs, lhs);
}

CiBit IIntOpGen::lower_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !greater(lhs, rhs);
}

CiBit IIntOpGen::greater_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !lower(lhs, rhs);
}
