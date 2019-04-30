#include <int_op_gen/impl/negate.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector Negate::oper(const CiBitVector& a) const {
  return add(~a, encode_plain_int(1, a.size()));
}
