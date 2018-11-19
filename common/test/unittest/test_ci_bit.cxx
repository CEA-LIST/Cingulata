#include <gtest/gtest.h>

#include <bit_exec/interface.hxx>
#include <ci_bit.hxx>

using namespace std;
using namespace cingulata;

TEST(CiBit_PlainText, Op2Inp) {
  const int tt[4][10] =
  {
  //   and nand    andny andyn or    nor   orny  oryn  xor   xnor   bool function
                                                                    /* x y */
    {  0,    1,    0,    0,    0,    1,    1,    1,    0,    1   }, /* 0 0 */
    {  0,    1,    1,    0,    1,    0,    1,    0,    1,    0   }, /* 0 1 */
    {  0,    1,    0,    1,    1,    0,    0,    1,    1,    0   }, /* 1 0 */
    {  1,    0,    0,    0,    1,    0,    1,    1,    0,    1   }  /* 1 1 */
  };

  pair<string,std::function<void (CiBit&, const CiBit&)>> operators[] {
    { "op_and",   [](CiBit& a, const CiBit& b) { a.op_and(b);   } },
    { "op_nand",  [](CiBit& a, const CiBit& b) { a.op_nand(b);  } },
    { "op_andny", [](CiBit& a, const CiBit& b) { a.op_andny(b); } },
    { "op_andyn", [](CiBit& a, const CiBit& b) { a.op_andyn(b); } },
    { "op_or",    [](CiBit& a, const CiBit& b) { a.op_or(b);    } },
    { "op_nor",   [](CiBit& a, const CiBit& b) { a.op_nor(b);   } },
    { "op_orny",  [](CiBit& a, const CiBit& b) { a.op_orny(b);  } },
    { "op_oryn",  [](CiBit& a, const CiBit& b) { a.op_oryn(b);  } },
    { "op_xor",   [](CiBit& a, const CiBit& b) { a.op_xor(b);   } },
    { "op_xnor",  [](CiBit& a, const CiBit& b) { a.op_xnor(b);  } }
  };

  for (int idx = 0; idx < 10; ++idx)
  {
    for (int v = 0; v < 4; ++v) {
      CiBit a(1 & (v >> 1));
      CiBit b(1 & v);
      operators[idx].second(a,b);
      ASSERT_EQ(tt[v][idx], a.get_val()) << " operator " << operators[idx].first;
    }
  }
}
