#ifndef INT_OP_GEN_MULT_DEPTH
#define INT_OP_GEN_MULT_DEPTH

#include <int_op_gen/interface.hxx>
#include <int_op_gen/impl/all.hxx>

namespace cingulata
{
  class IntOpGenDepth : public IIntOpGen {
  public:
    IntOpGenDepth();

    virtual CiBitVector add     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBitVector neg     ( const CiBitVector& lhs) const override;

    virtual CiBitVector mul     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit equal         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit lower         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

  private:
    int_ops::SklanskyAdder m_add;
    int_ops::Negate           m_neg;
    int_ops::Multiplier       m_mul;
    int_ops::EqualDepth       m_equal;
    int_ops::LowerCompDepth   m_lower;
  };
}

#endif
