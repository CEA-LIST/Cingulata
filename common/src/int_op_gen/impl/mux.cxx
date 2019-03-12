#include <int_op_gen/impl/mux.hxx>
#include <int_op_gen/impl/dec.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;
using namespace std;

CiBitVector MuxDepth::oper(const CiBitVector& cond, const vector<CiBitVector>& inps) const {
  const static Decoder decoder;
  const unsigned n = inps[0].size();

  CiBitVector dec_cond = decoder(cond);

  CiBitVector res(n, 0);
  for (unsigned i = 0; i < dec_cond.size(); ++i) {
    res ^= inps[i] & CiBitVector(n, dec_cond[i]);
  }

  return res;
}
