#ifndef NEGATE_OPER
#define NEGATE_OPER

#include <functional>

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      2's complement negation operator
     */
    class Negate : public UnaryOper {
    public:
      Negate(const std::function<BinaryOper::signature>& add_p) : add(add_p) {}

    private:
      /**
       * @brief      Implementation
       *
       * @param[in]  a     bit-vector to negate
       *
       * @return     2's complement negated bit-vector
       */
      CiBitVector oper(const CiBitVector& a) const override;

      std::function<BinaryOper::signature> add;
    };
  }
}

#endif
