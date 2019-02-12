#ifndef INT_OP_GEN_INTERFACE
#define INT_OP_GEN_INTERFACE

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>

namespace cingulata
{
  class IIntOpGen {
  public:
    /**
     * @brief      Add integers represented as vectors of bits. The output
     *             vector size is equal to the @c lhs input vector size
     *
     * @param[in]  lhs   The left hand side bit vector
     * @param[in]  rhs   The right hand side bit vector
     *
     * @return     The sum of input bit vectors as a bit vector
     */
    virtual CiBitVector add     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBitVector sub     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBitVector neg     ( const CiBitVector& lhs) const = 0;

    virtual CiBitVector mul     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBitVector square  ( const CiBitVector& lhs) const;


    virtual CiBit not_equal     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBit equal         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit lower         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBit lower_equal   ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit greater_equal ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit greater       ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;
  };

  template<typename _impl>
  class IntOpGen : public IIntOpGen {
  public:
    virtual CiBitVector add     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBitVector neg     ( const CiBitVector& lhs) const override;

    virtual CiBitVector mul     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit not_equal     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

    virtual CiBit lower         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const override;

  private:
    _impl impl_m;
  };

  #include "interface_defs.cxx"
}

#endif
