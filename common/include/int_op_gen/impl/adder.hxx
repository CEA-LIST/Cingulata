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
/**
 * @brief      Sklansky adder operator (minimum multiplicative depth)
 * @details    The multiplicative depth of generated circuit is log2( lhs.size())+1 @c
 *             It has @c lhs.size()*log(lhs.size()) AND gates and @
 *             4*lhs.size()-1 XOR gates.
 * @note       Circuit described in David Harris.
 *             *A taxonomy of Parallel Prefix Networks*
 */
class SklanskyAdder : public BinaryOper {
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

/**
 * @brief      Adds together several bit vectors. Inputs bit-sizes can be
 *             different. The smallest possible bit-sized output bit vector is
 *             returned (e.g. for @c n inputs of same size @c m the output size
 *             is @c m+ceil(log2(n))).
 */
class MultiInputAdder : public NaryOper {
public:
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  p_bin_adder  Binary adder to use for adding 2 values
   */
  MultiInputAdder(const std::function<BinaryOper::signature>& p_bin_adder);
private:

  /**
   * @brief      Implementation
   *
   * @param[in]  inps  Array of bit vectors to add
   *
   * @return     Sum of input bit vectors
   */
  CiBitVector oper(const std::vector<CiBitVector> &inps) const override;

  std::function<BinaryOper::signature> bin_adder;
};

} // namespace int_ops
} // namespace cingulata

#endif
