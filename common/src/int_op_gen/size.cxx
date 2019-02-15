#include <int_op_gen/size.hxx>

using namespace cingulata;

IntOpGenSize::IntOpGenSize()
:
  m_neg{m_add}
{}

CiBitVector IntOpGenSize::add(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_add(lhs, rhs);
}

CiBitVector IntOpGenSize::neg(const CiBitVector& lhs) const
{
  return m_neg(lhs);
}

CiBitVector IntOpGenSize::mul(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_mul(lhs, rhs);
}

CiBit IntOpGenSize::equal(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_equal(lhs, rhs);
}

CiBit IntOpGenSize::lower(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_lower(lhs, rhs);
}
