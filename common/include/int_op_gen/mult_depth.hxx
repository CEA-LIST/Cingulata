#ifndef INT_OP_GEN_MULT_DEPTH
#define INT_OP_GEN_MULT_DEPTH

#include "int_op_gen/interface.hxx"

namespace cingulata
{
  class IntOpGenDepth : public IIntOpGen {
  public:
    virtual CiBit not_equal ( const BitVectorT& lhs,
                              const BitVectorT& rhs)          const override;
  };
}

#endif
