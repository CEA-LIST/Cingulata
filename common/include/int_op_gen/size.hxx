#ifndef INT_OP_GEN_SIZE
#define INT_OP_GEN_SIZE

#include "int_op_gen/interface.hxx"

namespace cingulata
{
  class IntOpGenSize : public IIntOpGen {
  public:
    virtual CiBit not_equal ( const BitVectorT& lhs,
                              const BitVectorT& rhs)          const override;

    virtual CiBit compare   ( const BitVectorT& lhs,
                              const BitVectorT& rhs,
                              const int carry_inp = 0,
                              const bool signed_comp = false) const override;
  };
}

#endif
