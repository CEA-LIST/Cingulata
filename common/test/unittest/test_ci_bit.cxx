#include <gtest/gtest.h>

#include <bit_exec/interface.hxx>
#include <ci_bit.hxx>

using namespace std;
using namespace cingulata;

tuple<
  string,
  std::function<void (CiBit&, const CiBit&)>,
  vector<int>
> obj_operators[] =
{
  { "op_not",   [](CiBit& a, const CiBit& b) { a.op_not();    },  {1,1,0,0} },
  { "op_and",   [](CiBit& a, const CiBit& b) { a.op_and(b);   },  {0,0,0,1} },
  { "op_nand",  [](CiBit& a, const CiBit& b) { a.op_nand(b);  },  {1,1,1,0} },
  { "op_andny", [](CiBit& a, const CiBit& b) { a.op_andny(b); },  {0,1,0,0} },
  { "op_andyn", [](CiBit& a, const CiBit& b) { a.op_andyn(b); },  {0,0,1,0} },
  { "op_or",    [](CiBit& a, const CiBit& b) { a.op_or(b);    },  {0,1,1,1} },
  { "op_nor",   [](CiBit& a, const CiBit& b) { a.op_nor(b);   },  {1,0,0,0} },
  { "op_orny",  [](CiBit& a, const CiBit& b) { a.op_orny(b);  },  {1,1,0,1} },
  { "op_oryn",  [](CiBit& a, const CiBit& b) { a.op_oryn(b);  },  {1,0,1,1} },
  { "op_xor",   [](CiBit& a, const CiBit& b) { a.op_xor(b);   },  {0,1,1,0} },
  { "op_xnor",  [](CiBit& a, const CiBit& b) { a.op_xnor(b);  },  {1,0,0,1} },

  { "oper +=",  [](CiBit& a, const CiBit& b) { a += b; },         {0,1,1,0} },
  { "oper -=",  [](CiBit& a, const CiBit& b) { a -= b; },         {0,1,1,0} },
  { "oper *=",  [](CiBit& a, const CiBit& b) { a *= b; },         {0,0,0,1} },
  { "oper &=",  [](CiBit& a, const CiBit& b) { a &= b; },         {0,0,0,1} },
  { "oper |=",  [](CiBit& a, const CiBit& b) { a |= b; },         {0,1,1,1} },
  { "oper ^=",  [](CiBit& a, const CiBit& b) { a ^= b; },         {0,1,1,0} },
                                                             // b  0 1 0 1
                                                             // a  0 0 1 1
  { "", nullptr, {}}
};

TEST(CiBit_PT, oper_2_cibit_input) {

  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(obj_operators[idx]);
    auto& op_func = get<1>(obj_operators[idx]);
    auto& op_tt = get<2>(obj_operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int b1 = 1 & (v >> 1);
      CiBit a(b1);
      int b0 = 1 & (v >> 0);
      CiBit b(b0);

      op_func(a, b);

      // operator result is good
      ASSERT_EQ(op_tt[v], (int)a.get_val())
        << " operator '" << op_name << "'" << " (" << b1 << "," << b0 << ")";

      // value of b does not change
      ASSERT_EQ(b0, (int)b.get_val()) << " operator '" << op_name << "'";

      // stays plain afterwards
      ASSERT_TRUE(a.is_plain()) << " operator '" << op_name << "'";
      ASSERT_TRUE(b.is_plain()) << " operator '" << op_name << "'";
    }
  }
}

TEST(CiBit_PT, oper_1_cibit_1_pt_input) {

  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(obj_operators[idx]);
    auto& op_func = get<1>(obj_operators[idx]);
    auto& op_tt = get<2>(obj_operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int b1 = 1 & (v >> 1);
      CiBit a(b1);

      int b0 = 1 & (v >> 0);

      op_func(a,b0);

      ASSERT_EQ(op_tt[v], (int)a.get_val())
        << " operator '" << op_name << "'" << " (" << b1 << "," << b0 << ")";

      // stays plain afterwards
      ASSERT_TRUE(a.is_plain()) << " operator '" << op_name << "'";
    }
  }
}

tuple<
  string,
  std::function<CiBit (const CiBit&, const CiBit&)>,
  vector<int>
> operators[] =
{
  { "oper + ",  [](const CiBit& a, const CiBit& b) { return a + b;  },  {0,1,1,0} },
  { "oper - ",  [](const CiBit& a, const CiBit& b) { return a - b;  },  {0,1,1,0} },
  { "oper * ",  [](const CiBit& a, const CiBit& b) { return a * b;  },  {0,0,0,1} },
  { "oper ~ ",  [](const CiBit& a, const CiBit& b) { return    ~a;  },  {1,1,0,0} },
  { "oper ^ ",  [](const CiBit& a, const CiBit& b) { return a ^ b;  },  {0,1,1,0} },
  { "oper & ",  [](const CiBit& a, const CiBit& b) { return a & b;  },  {0,0,0,1} },
  { "oper | ",  [](const CiBit& a, const CiBit& b) { return a | b;  },  {0,1,1,1} },

  { "oper ! ",  [](const CiBit& a, const CiBit& b) { return    !a;  },  {1,1,0,0} },
  { "oper &&",  [](const CiBit& a, const CiBit& b) { return a && b; },  {0,0,0,1} },
  { "oper ||",  [](const CiBit& a, const CiBit& b) { return a || b; },  {0,1,1,1} },

  { "oper ==",  [](const CiBit& a, const CiBit& b) { return a == b; },  {1,0,0,1} },
  { "oper !=",  [](const CiBit& a, const CiBit& b) { return a != b; },  {0,1,1,0} },
  { "oper < ",  [](const CiBit& a, const CiBit& b) { return a < b;  },  {0,1,0,0} },
  { "oper <= ", [](const CiBit& a, const CiBit& b) { return a <= b; },  {1,1,0,1} },
  { "oper > ",  [](const CiBit& a, const CiBit& b) { return a > b;  },  {0,0,1,0} },
  { "oper >=",  [](const CiBit& a, const CiBit& b) { return a >= b; },  {1,0,1,1} },
                                                                   // b  0 1 0 1
                                                                   // a  0 0 1 1

  { "", nullptr, {}}
};

TEST(CiBit_PT, ext_oper_2_cibit_input) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(operators[idx]);
    auto& op_func = get<1>(operators[idx]);
    auto& op_tt = get<2>(operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int b1 = 1 & (v >> 1);
      CiBit a(b1);
      int b0 = 1 & (v >> 0);
      CiBit b(b0);

      CiBit c = op_func(a, b);

      // operator result is good
      ASSERT_EQ(op_tt[v], (int)c.get_val())
        << " operator '" << op_name << "'" << " (" << b1 << "," << b0 << ")";

      // value of b does not change
      ASSERT_EQ(b1, (int)a.get_val()) << " operator '" << op_name << "'";
      ASSERT_EQ(b0, (int)b.get_val()) << " operator '" << op_name << "'";

      // stays plain afterwards
      ASSERT_TRUE(a.is_plain()) << " operator '" << op_name << "'";
      ASSERT_TRUE(b.is_plain()) << " operator '" << op_name << "'";
      ASSERT_TRUE(c.is_plain()) << " operator '" << op_name << "'";
    }
  }
}

TEST(CiBit, set_name) {
  CiBit a;
  string test_name = "lol";

  a.set_name(test_name);
  ASSERT_EQ(test_name, a.get_name());

  a.set_name(test_name + test_name);
  ASSERT_EQ(test_name + test_name, a.get_name());
}

TEST(CiBit, clr_name) {
  CiBit a;
  string test_name = "lol";
  a.set_name(test_name);
  a.clr_name();
  ASSERT_NE(test_name, a.get_name());
}

