#ifndef ADDER_OPER
#define ADDER_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Riple-carry adder operator (small size)
     * @details    The multiplicative depth of generated circuit is @c
     *             lhs.size(). It has @c lhs.size() AND gates and @c
     *             4*lhs.size()-1 XOR gates.
     * @note       Circuit described in Kolesnikov V, Sadeghi AR, Schneider T.
     *             *Improved garbled circuit building blocks and applications to
     *             auctions and computing minima*
     */
    class RippleCarryAdder : public BinaryOper {
      /**
       * @brief      Implementations
       * @details    The size of result is the size of @c lhs.
       *
       * @param[in]  lhs   The left hand side
       * @param[in]  rhs   The right hand side
       *
       * @return     sum bit-vector
       */
      CiBitVector oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;
    };
  }
}
#endif
