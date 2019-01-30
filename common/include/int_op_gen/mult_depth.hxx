#ifndef INT_OP_GEN_MULT_DEPTH
#define INT_OP_GEN_MULT_DEPTH

#include "int_op_gen/interface.hxx"

namespace cingulata
{
  class IntOpGenDepth : public IIntOpGen {
  public:
    virtual CiBit not_equal ( const CiBitVector& lhs,
                              const CiBitVector& rhs) const override;

    virtual CiBit lower     ( const CiBitVector& lhs,
                              const CiBitVector& rhs) const override;
  };
}

#endif
