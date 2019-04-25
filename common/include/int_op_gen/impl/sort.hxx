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
     * @brief      sort an array of CInt
     * @details    The sort function sort a vector of Cint and return the values.
     *             The multiplicative depth of this implementation is minimal.
     * @note       Implementation described by Cetin & al in
     *             *Low Dpeth Circuits for Efficient Homomorphic Sorting*.
     */
    class SortDepth : public SortOper {
    public:
      SortDepth(const std::function<CompOper::signature>& comp,
               const std::function<CompOper::signature>& equal,
               const std::function<BinaryOper::signature>& adder_p)
                : cmp(comp), equ(equal), adder(adder_p) {}


    private:

      std::vector<CiBitVector> oper(const std::vector<CiBitVector>& v_cbv,
                                    const std::vector<CiBitVector>& i_cbv,
                                    const bool reverse) const;

      CiBitVector hammingWeight(const CiBitVector x, unsigned int l, unsigned int size_ham ) const;

      std::function<CompOper::signature> cmp;

      std::function<CompOper::signature> equ;

      std::function<BinaryOper::signature> adder;

    };

  }
}
#endif