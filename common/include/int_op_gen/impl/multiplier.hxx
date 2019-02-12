#ifndef MULTIPLIER_OPER
#define MULTIPLIER_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Multiplier implementations
     */
    class Multiplier : public BinaryOper {
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Product between inputs
       */
      CiBitVector oper(const CiBitVector& lhs, const CiBitVector& rhs) const override {
        const int size = lhs.size();
        CiBitVector res(size);

        /** @todo implement */
        return res;
      }
    };
  }
}

#endif
