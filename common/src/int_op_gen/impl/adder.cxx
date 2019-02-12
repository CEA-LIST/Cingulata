#include <int_op_gen/impl/adder.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector RippleCarryAdder::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();
  CiBitVector res(size);

  CiBit carry{0};
  for (int i = 0; i < size; ++i) {
    CiBit n1 = carry ^ lhs[i];
    CiBit n2 = carry ^ rhs[i];
    res[i] = n1 ^ rhs[i];
    if (i < size-1)
      carry = (n1 & n2) ^ carry;
  }

  return res;
}
