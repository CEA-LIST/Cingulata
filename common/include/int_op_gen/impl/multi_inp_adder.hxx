#ifndef MULTI_INP_ADDER_OPER
#define MULTI_INP_ADDER_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
namespace int_ops
{
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

  CiBitVector add(const std::vector<CiBitVector> &inps, const unsigned max_bit_size = -1) const;

  std::function<BinaryOper::signature> bin_adder;
};

} // namespace int_ops
} // namespace cingulata

#endif
