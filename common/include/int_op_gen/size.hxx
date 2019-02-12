#ifndef INT_OP_GEN_SIZE
#define INT_OP_GEN_SIZE

#include <int_op_gen/interface.hxx>
#include <int_op_gen/impl/all.hxx>

namespace cingulata
{
  struct IntOpGenSize_impl {
    int_ops::RippleCarryAdder add;
    int_ops::Negate           neg{add};
    int_ops::Multiplier       mul;
    int_ops::NotEqualSize     not_equal;
    int_ops::LowerCompSize    lower;
  };

  class IntOpGenSize : public IntOpGen<IntOpGenSize_impl> {};
}

#endif
