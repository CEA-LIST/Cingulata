#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_context.hxx>
#include <ci_fncs.hxx>
#include <ci_int.hxx>

using namespace cingulata;

CiInt cingulata::select(const CiBit &cond, const CiInt &a, const CiInt &b) {
  return CiContext::get_int_op_gen()->mux(cond, a.cast(), b.cast());
}
