#ifndef INT_OP_GEN_INTERFACE
#define INT_OP_GEN_INTERFACE

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_int.hxx>
#include <int_op_gen/impl/all.hxx>
#include <int_op_gen/interface.hxx>

#include <vector>

namespace cingulata
{
  class IIntOpGen {
  public:
    IIntOpGen();

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


    virtual CiBit equal         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBit not_equal     ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit lower         ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const = 0;

    virtual CiBit lower_equal   ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit greater_equal ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBit greater       ( const CiBitVector& lhs,
                                  const CiBitVector& rhs) const;

    virtual CiBitVector mux     ( const CiBit &cond,
                                  const CiBitVector &a,
                                  const CiBitVector &b) const;

    virtual CiBitVector mux     ( const CiBitVector &cond,
                                  const std::vector<CiBitVector> &inps) const;


    virtual std::vector<CiBitVector> sort (const std::vector<CiBitVector> &v_cbv,
                                        const std::vector<CiBitVector> &i_cbv,
                                        const bool reverse) const;

  private:
    int_ops::MuxDepth           m_mux;
    int_ops::SortDepth          m_sort;
  };
}

#endif
