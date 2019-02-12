#ifndef LOWER_OPER
#define LOWER_OPER

#include <functional>

#include <int_op_gen/impl/operator.hxx>

namespace cingulata {
  namespace int_ops {
    /**
     * @brief      Logarithmic depth lower comparator
     * @details    The multiplicative depth of generated circuit is logaritmic
     *             in input size.
     * @note       Circuit described in Garay J, Schoenmakers B, Villegas J.
     *             *Practical and secure solutions for integer comparison*
     */
    class LowerCompDepth : public CompOper {
    public:
      LowerCompDepth(const std::function<CompOper::signature>& equal_p) : equal(equal_p) {}

    private:
      /**
       * @brief      Implementation
       *
       * @param[in]  lhs    The left hand side
       * @param[in]  rhs    The right hand side
       *
       * @return     Comparison result
       */
      CiBit oper(const CiBitVector& lhs, const CiBitVector& rhs) const override;

      std::function<CompOper::signature> equal;
    };


    /**
     * @brief      Small size lower comparator
     * @details    The multiplicative depth of generated circuit is linear in
     *             input size. It uses @c lhs.size() AND gates and @c
     *             3*lhs.size() XOR gates
     * @note       Circuit described in Kolesnikov V, Sadeghi AR, Schneider T.
     *             *Improved garbled circuit building blocks and applications to
     *             auctions and computing minima*
     */
    class LowerCompSize : public CompOper {
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
