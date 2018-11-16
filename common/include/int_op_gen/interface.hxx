#ifndef INT_OP_GEN_INTERFACE
#define INT_OP_GEN_INTERFACE

#include <bit_exec/interface.hxx>

#include <vector>

namespace cingulata
{
  class IIntOpGen {
  public:
    using BitVectorT = std::vector<CiBit>;

    /**
     * @brief      Add integers represented as bit vectors. The output bit
     *             vector size is equal to the @c lhs size
     *
     * @param[in]  lhs   The left hand side bit vector
     * @param[in]  rhs   The right hand side bit vector
     *
     * @return     The sum of input bit vectors as a bit vector
     */
    BitVectorT& add             (const BitVectorT& lhs, const BitVectorT& rhs) const;
    BitVectorT& sub             (const BitVectorT& lhs, const BitVectorT& rhs) const;
    BitVectorT& mul             (const BitVectorT& lhs, const BitVectorT& rhs) const;
    BitVectorT& square          (const BitVectorT& lhs) const;

    CiBit&      not_equal       (const BitVectorT& lhs, const BitVectorT& rhs) const;
    CiBit&      equal           (const BitVectorT& lhs, const BitVectorT& rhs) const;
    CiBit&      lower           (const BitVectorT& lhs, const BitVectorT& rhs) const;
    CiBit&      lower_equal     (const BitVectorT& lhs, const BitVectorT& rhs) const;
    CiBit&      greater_equal   (const BitVectorT& lhs, const BitVectorT& rhs) const;
    CiBit&      greater         (const BitVectorT& lhs, const BitVectorT& rhs) const;

  };
}

#endif
