#include <int_op_gen/mult_depth.hxx>

using namespace std;
using namespace cingulata;

IntOpGenDepth::IntOpGenDepth()
:
  m_neg { m_add },
  m_mul { m_add },
  m_lower { m_equal }
{
}

CiBitVector IntOpGenDepth::add(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_add(lhs, rhs);
}

CiBitVector IntOpGenDepth::neg(const CiBitVector& lhs) const
{
  return m_neg(lhs);
}

CiBitVector IntOpGenDepth::mul(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_mul(lhs, rhs);
}

CiBit IntOpGenDepth::equal(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_equal(lhs, rhs);
}

CiBit IntOpGenDepth::lower(const CiBitVector& lhs, const CiBitVector& rhs) const
{
  return m_lower(lhs, rhs);
}
