#ifndef MULTIPLIER_OPER
#define MULTIPLIER_OPER

#include <int_op_gen/impl/operator.hxx>

#include <functional>
#include <cassert>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Wallace Multiplier implementations
     */
    class WallaceMultiplier : public BinaryOper {
      public:
        WallaceMultiplier(const std::function<BinaryOper::signature>& adder_p) : adder(adder_p) {}

      private:
      /**
       * @brief      Wallace multiplier
       *
       * @details    lhs and rhs have the same length in bits
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Product between inputs, having the same bit length as inputs
       */
      CiBitVector oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;

      std::function<BinaryOper::signature> adder;
   };
  }
}

#endif
