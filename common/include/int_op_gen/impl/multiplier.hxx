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

      /**
       * @brief      Generic Wallace multiplier
       *
       * @details    lhs and rhs do not have the same length
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Product between inputs, the output length is size(lhs)+size(rhs)-1
       */
      CiBitVector multiply(const CiBitVector& lhs, const CiBitVector& rhs) const;

      /**
       * @brief      Wallace tree reduction using full-adder
       *
       * @details    In our implemntation, we do not require half-adder as it
       *             is replaced by a full-adder with CiBit::zero inputs
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Product between inputs
       */
      std::vector<CiBitVector> reduce(const std::vector<CiBitVector>& tree) const;
    };
  }
}

#endif
