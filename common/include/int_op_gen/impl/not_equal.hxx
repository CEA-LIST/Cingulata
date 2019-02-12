#ifndef NOT_EQUAL_OPER
#define NOT_EQUAL_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {
    /**
     * @brief      Small depth inequality comparator
     * @details    The multiplicative depth of generated circuit is logaritmic
     *             in input size. It uses @c lhs.size()-1 AND gates and @c
     *             lhs.size() XOR gates.
     */
    class NotEqualDepth : public CompOper {
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Comparison result
       */
      CiBit oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;
    };

    /**
     * @brief      Small size inequality comparator
     * @details    The multiplicative depth of generated circuit is linear in
     *             input size. It uses @c lhs.size() AND gates and @c
     *             3*lhs.size() XOR gates
     * @note       Circuit described in Kolesnikov V, Sadeghi AR, Schneider T.
     *             *Improved garbled circuit building blocks and applications to
     *             auctions and computing minima*
     */
    class NotEqualSize : public CompOper {
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     Comparison result
       */
      CiBit oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;
    };
  }
}
#endif
