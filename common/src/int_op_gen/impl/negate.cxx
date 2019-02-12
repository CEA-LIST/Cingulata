#include <int_op_gen/impl/negate.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector Negate::oper(const CiBitVector& a) const {
  CiBitVector tmp = ~a;
  CiBitVector one = CiBitVector(tmp.size());
  one[0] = CiBit::one;
  return add(tmp, one);
}
