#ifndef MULTIPLIER_OPER
#define MULTIPLIER_OPER

#include <cassert>
#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Wallace Multiplier implementations
     */
    class WallaceMultiplier : public BinaryOper {
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
   };
  }
}

#endif
