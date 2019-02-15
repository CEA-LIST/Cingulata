#ifndef INT_OP_GEN_SIZE
#define INT_OP_GEN_SIZE

#include <int_op_gen/interface.hxx>
#include <int_op_gen/impl/all.hxx>

namespace cingulata
{
  class IntOpGenSize : public IIntOpGen {
  public:
    IntOpGenSize();

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
    int_ops::RippleCarryAdder m_add;
    int_ops::Negate           m_neg;
    int_ops::Multiplier       m_mul;
    int_ops::EqualSize        m_equal;
    int_ops::LowerCompSize    m_lower;
  };
}

#endif
