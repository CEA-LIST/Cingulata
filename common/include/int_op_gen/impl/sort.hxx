#ifndef SORT_OPER
#define SORT_OPER

#include <int_op_gen/impl/operator.hxx>

#include <functional>
#include <cassert>

namespace cingulata
{
namespace int_ops
{

/**
 * @brief      Sort an array of integers
 * @details    The sort function sort a vector of CiInt and return the values.
 *             The multiplicative depth of this implementation is minimal.
 * @note       Implementation described by Cetin & al in *Low Depth Circuits for
 *             Efficient Homomorphic Sorting*.
 */
class SortDepth : public SortOper {
public:
  SortDepth(const std::function<CompOper::signature> &cmp,
            const std::function<CompOper::signature> &equ,
            const std::function<BinaryOper::signature> &adder)
      : cmp(cmp), equ(equ), adder(adder) {}

private:
  std::vector<CiBitVector> oper(const std::vector<CiBitVector> &v_cbv,
                                const std::vector<CiBitVector> &i_cbv,
                                const bool reverse) const;

  CiBitVector hammingWeight(const CiBitVector x, unsigned int l,
                            unsigned int size_ham) const;

  std::function<CompOper::signature> cmp;

  std::function<CompOper::signature> equ;

  std::function<BinaryOper::signature> adder;
};
}
}
#endif
