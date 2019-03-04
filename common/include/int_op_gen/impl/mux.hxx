#ifndef MUX_OPER
#define MUX_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      Multiplexer operator (small depth)
     * @details    The multiplicative depth of generated circuit is @c
     *             ceil(log2(ceil(log2(m)))) + 1. It has approximatively @c
     *             2m+mn AND gates. Here @c m is the number of inputs and @c n
     *             the input word bit-size.
     * @note       Circuit MUX_{DNFs} from Buescher N, Holzer A, Weber A,
     *             Katzenbeisser S. *Compiling low depth circuits for practical
     *             secure computation*
     */
    class MuxDepth : public MuxOper {
      CiBitVector oper(const CiBitVector& cond, const std::vector<CiBitVector>& inps) const;
    };
  }
}
#endif
