#include <gtest/gtest.h>

#include <bit_exec/interface.hxx>
#include <ci_bit.hxx>

#include "bit_exec_plain.hxx"

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


class CiBitOper : public ::testing::TestWithParam<tuple<bool,bool>> {
public:
  bool encrypt_1st;
  bool encrypt_2nd;
  static IBitExec* be;

  static void SetUpTestCase() {
    be = new BitExecFake();
    CiBit::set_bit_exec(be);
  }

  static void TearDownTestCase() {
    delete be;
  }

  virtual void SetUp() {
    tie(encrypt_1st, encrypt_2nd) = GetParam();
  }

};
IBitExec* CiBitOper::be;

TEST_P(CiBitOper, two_input) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(obj_operators[idx]);
    auto& op_func = get<1>(obj_operators[idx]);
    auto& op_tt = get<2>(obj_operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int a_val_inp = 1 & (v >> 1);
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();

      int b_val_inp = 1 & (v >> 0);
      CiBit b(b_val_inp);
      if (encrypt_2nd) b.encrypt();

      op_func(a, b);

      int a_val_out = a.decrypt();
      int b_val_out = encrypt_2nd ? b.decrypt() : b.get_val();

      // operator result is good
      EXPECT_EQ(op_tt[v], a_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      EXPECT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";

      // stays plain afterwards
      EXPECT_TRUE(encrypt_2nd or b.is_plain()) << " operator '" << op_name << "'";
    }
  }
}


TEST_P(CiBitOper, two_input_pt) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(obj_operators[idx]);
    auto& op_func = get<1>(obj_operators[idx]);
    auto& op_tt = get<2>(obj_operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int a_val_inp = 1 & (v >> 1);
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();

      int b_val_inp = 1 & (v >> 0);
      int b_val_out = b_val_inp;

      op_func(a, b_val_out);

      int a_val_out = a.decrypt();

      // operator result is good
      EXPECT_EQ(op_tt[v], a_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      EXPECT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";
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

TEST_P(CiBitOper, two_input_ext) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(operators[idx]);
    auto& op_func = get<1>(operators[idx]);
    auto& op_tt = get<2>(operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int a_val_inp = 1 & (v >> 1);
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();

      int b_val_inp = 1 & (v >> 0);
      CiBit b(b_val_inp);
      if (encrypt_2nd) b.encrypt();

      CiBit c = op_func(a, b);

      int a_val_out = encrypt_1st ? a.decrypt() : a.get_val();
      int b_val_out = encrypt_2nd ? b.decrypt() : b.get_val();
      int c_val_out = c.decrypt();

      // operator result is good
      EXPECT_EQ(op_tt[v], c_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      EXPECT_EQ(a_val_inp, a_val_out) << " operator '" << op_name << "'";
      EXPECT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";

      // stays plain afterwards
      EXPECT_TRUE(encrypt_1st or a.is_plain()) << " operator '" << op_name << "'";
      EXPECT_TRUE(encrypt_2nd or b.is_plain()) << " operator '" << op_name << "'";
    }
  }
}

INSTANTIATE_TEST_CASE_P(,
                        CiBitOper,
                        ::testing::Combine(::testing::Bool(), ::testing::Bool()));


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
