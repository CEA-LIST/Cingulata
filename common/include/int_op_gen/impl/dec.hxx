#ifndef DEC_OPER
#define DEC_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
  namespace int_ops
  {

    /**
     * @brief      1-of-n decoder
     * @details    The decoder operator transforms an n-bit input word into a
     *             2^n-bit vector with a single bit set to one and others to
     *             zero. The set bit is the bit whose index is given by integer
     *             value of n-bit input word.
     * @note       The multiplicative depth of generated circuit is @c
     *             ceil(log2(n)). It has approximatively @c n.2^n AND gates. Here
     *             @c n is the number of input bits to decode.
     */
    class Decoder : public UnaryOper {
      CiBitVector oper(const CiBitVector& inp) const;
    };
  }
}
#endif
