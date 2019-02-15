#include <int_op_gen/interface.hxx>

using namespace cingulata;

CiBitVector IIntOpGen::sub(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return add(lhs, neg(rhs));
}

CiBitVector IIntOpGen::square(const CiBitVector& rhs) const {
  return mul(rhs, rhs);
}

CiBit IIntOpGen::not_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !equal(lhs,rhs);
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
