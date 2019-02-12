#ifndef COMMON_OPER
#define COMMON_OPER

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Unary operator base class
     */
    class UnaryOper
    {
    public:
      using signature = CiBitVector (const CiBitVector& rhs);

      CiBitVector operator()(const CiBitVector& rhs) const;

    private:
      virtual CiBitVector oper(const CiBitVector&) const = 0;
    };

    /**
     * @brief      Binary operator base class
     */
    class BinaryOper
    {
    public:
      using signature = CiBitVector (const CiBitVector&, const CiBitVector&);

      CiBitVector operator()(const CiBitVector& lhs, const CiBitVector& rhs) const;

    private:
      virtual CiBitVector oper(const CiBitVector&, const CiBitVector&) const = 0;
    };

    /**
     * @brief      Comparison operator base class
     */
    class CompOper
    {
    public:
      using signature = CiBit (const CiBitVector&, const CiBitVector&);

      CiBit operator()(const CiBitVector& lhs, const CiBitVector& rhs) const;

    private:
      virtual CiBit oper(const CiBitVector&, const CiBitVector&) const = 0;
    };
  }
}
#endif
