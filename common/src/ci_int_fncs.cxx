#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_context.hxx>
#include <ci_int_fncs.hxx>
#include <ci_int.hxx>

using namespace std;

namespace cingulata {

CiInt select(const CiBit &cond, const CiInt &a, const CiInt &b) {
  return CiContext::get_int_op_gen()->mux(cond, b.cast(), a.cast());
}

CiInt sum(const vector<CiInt> &elems) {
  vector<CiBitVector> elems_bv;
  for (const CiInt& elem: elems)
    elems_bv.push_back(elem.cast());
  return CiContext::get_int_op_gen()->sum(elems_bv);
}

}
