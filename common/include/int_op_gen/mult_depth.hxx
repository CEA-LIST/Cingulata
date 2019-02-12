#ifndef INT_OP_GEN_MULT_DEPTH
#define INT_OP_GEN_MULT_DEPTH

#include <int_op_gen/interface.hxx>
#include <int_op_gen/impl/all.hxx>

namespace cingulata
{
  struct IntOpGenDepth_impl {
    int_ops::RippleCarryAdder add;
    int_ops::Negate           neg{add};
    int_ops::Multiplier       mul;
    int_ops::NotEqualDepth    not_equal;
    int_ops::LowerCompDepth   lower{not_equal};
  };

  class IntOpGenDepth : public IntOpGen<IntOpGenDepth_impl> {};
}

#endif
