#ifndef INT_OP_GEN_SIZE
#define INT_OP_GEN_SIZE

#include "int_op_gen/interface.hxx"

namespace cingulata
{
  class IntOpGenSize : public IIntOpGen {
  public:
    virtual CiBit not_equal   ( const CiBitVector& lhs,
                                const CiBitVector& rhs) const override;

    virtual CiBit lower       ( const CiBitVector& lhs,
                                const CiBitVector& rhs) const override;
  };
}

#endif
