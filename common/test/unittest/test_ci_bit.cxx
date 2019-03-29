#include <gtest/gtest.h>

#include <bit_exec/interface.hxx>
#include <ci_bit.hxx>
#include <ci_context.hxx>

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

  virtual void SetUp() {
    tie(encrypt_1st, encrypt_2nd) = GetParam();
  }
};

TEST_P(CiBitOper, two_input) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(obj_operators[idx]);
    auto& op_func = get<1>(obj_operators[idx]);
    auto& op_tt = get<2>(obj_operators[idx]);

    if (op_name.empty()) break;

    for (int v = 0; v < op_tt.size(); ++v) {
      int a_val_inp = 1 & (v >> 1);
      string a_name("A");
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();
      a.set_name(a_name);

      int b_val_inp = 1 & (v >> 0);
      string b_name("B");
      CiBit b(b_val_inp);
      if (encrypt_2nd) b.encrypt();
      b.set_name(b_name);

      op_func(a, b);

      int a_val_out = a.decrypt();
      int b_val_out = encrypt_2nd ? b.decrypt() : b.get_val();

      // operator result is good
      ASSERT_EQ(op_tt[v], a_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      ASSERT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";

      // stays plain afterwards
      ASSERT_TRUE(encrypt_2nd or b.is_plain()) << " operator '" << op_name << "'";

      // name do not change
      ASSERT_EQ(a.get_name(), a_name) << " operator '" << op_name << "'";;
      ASSERT_EQ(b.get_name(), b_name) << " operator '" << op_name << "'";;
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
      string a_name("A");
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();
      a.set_name(a_name);

      int b_val_inp = 1 & (v >> 0);
      int b_val_out = b_val_inp;

      op_func(a, b_val_out);

      int a_val_out = a.decrypt();

      // operator result is good
      ASSERT_EQ(op_tt[v], a_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      ASSERT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";

      // name do not change
      ASSERT_EQ(a.get_name(), a_name) << " operator '" << op_name << "'";;
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

  { "op_not   ",  [](const CiBit& a, const CiBit& b) { return op_not(a);      },  {1,1,0,0} },
  { "op_and   ",  [](const CiBit& a, const CiBit& b) { return op_and(a, b);   },  {0,0,0,1} },
  { "op_nand  ",  [](const CiBit& a, const CiBit& b) { return op_nand(a, b);  },  {1,1,1,0} },
  { "op_andny ",  [](const CiBit& a, const CiBit& b) { return op_andny(a, b); },  {0,1,0,0} },
  { "op_andyn ",  [](const CiBit& a, const CiBit& b) { return op_andyn(a, b); },  {0,0,1,0} },
  { "op_or    ",  [](const CiBit& a, const CiBit& b) { return op_or(a, b);    },  {0,1,1,1} },
  { "op_nor   ",  [](const CiBit& a, const CiBit& b) { return op_nor(a, b);   },  {1,0,0,0} },
  { "op_orny  ",  [](const CiBit& a, const CiBit& b) { return op_orny(a, b);  },  {1,1,0,1} },
  { "op_oryn  ",  [](const CiBit& a, const CiBit& b) { return op_oryn(a, b);  },  {1,0,1,1} },
  { "op_xor   ",  [](const CiBit& a, const CiBit& b) { return op_xor(a, b);   },  {0,1,1,0} },
  { "op_xnor  ",  [](const CiBit& a, const CiBit& b) { return op_xnor(a, b);  },  {1,0,0,1} },

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
      string a_name("A");
      CiBit a(a_val_inp);
      if (encrypt_1st) a.encrypt();
      a.set_name(a_name);

      int b_val_inp = 1 & (v >> 0);
      string b_name("B");
      CiBit b(b_val_inp);
      if (encrypt_2nd) b.encrypt();
      b.set_name(b_name);

      CiBit c = op_func(a, b);

      int a_val_out = encrypt_1st ? a.decrypt() : a.get_val();
      int b_val_out = encrypt_2nd ? b.decrypt() : b.get_val();
      int c_val_out = c.decrypt();

      // operator result is good
      ASSERT_EQ(op_tt[v], c_val_out)
        << " operator '" << op_name << "'" << " (" << a_val_inp << "," << b_val_inp << ")";

      // value of b does not change
      ASSERT_EQ(a_val_inp, a_val_out) << " operator '" << op_name << "'";
      ASSERT_EQ(b_val_inp, b_val_out) << " operator '" << op_name << "'";

      // stays plain afterwards
      ASSERT_TRUE(encrypt_1st or a.is_plain()) << " operator '" << op_name << "'";
      ASSERT_TRUE(encrypt_2nd or b.is_plain()) << " operator '" << op_name << "'";

      // name do not change
      ASSERT_EQ(a.get_name(), a_name) << " operator '" << op_name << "'";;
      ASSERT_EQ(b.get_name(), b_name) << " operator '" << op_name << "'";;
      ASSERT_TRUE(c.get_name().empty()) << " operator '" << op_name << "'";;
    }
  }
}

INSTANTIATE_TEST_CASE_P(,
                        CiBitOper,
                        ::testing::Combine(::testing::Bool(), ::testing::Bool()));


TEST(CiBit, set_name) {
  CiBit a;
  string a_name = "A";

  a.set_name(a_name);
  ASSERT_EQ(a.get_name(), a_name);

  a.set_name(a_name + a_name);
  ASSERT_EQ(a_name + a_name, a.get_name());
}

TEST(CiBit, clr_name) {
  CiBit a;
  string a_name = "A";
  a.set_name(a_name);
  a.clr_name();
  ASSERT_NE(a_name, a.get_name());
}

TEST(CiBit, name_new_obj) {
  /** plaintext constructor */
  {
    CiBit a(rand()%2);
    ASSERT_TRUE(a.get_name().empty()) << "pt";
  }

  /** plaintext constructor name */
  {
    string a_name = "A";
    CiBit a(rand()%2, a_name);
    ASSERT_EQ(a.get_name(), a_name) << "pt_name";
  }

  /** copy constructor */
  {
    string a_name = "A";
    CiBit a(rand()%2, a_name);

    CiBit b(a);
    ASSERT_EQ(a.get_name(), a_name) << "copy";
    ASSERT_TRUE(b.get_name().empty()) << "copy";
  }

  /** move constructor */
  {
    string a_name = "A";
    CiBit b(CiBit(rand()%2, a_name));

    ASSERT_EQ(b.get_name(), a_name) << "move";
  }
}

TEST(CiBit, name_assign) {
  /** assign */
  {
    string a_name = "A";
    CiBit a(rand()%2, a_name);

    string b_name = "B";
    CiBit b(rand()%2, b_name);
    b = a;

    ASSERT_EQ(a.get_name(), a_name) << "assign";
    ASSERT_TRUE(b.get_name().empty()) << "assign";
  }

  /** move-assign */
  {
    CiBit b;

    string a_name = "A";
    b = CiBit(rand()%2, a_name);

    ASSERT_EQ(b.get_name(), a_name) << "move-assign";
  }
}

tuple<
  string,
  std::function<void (CiBit&)>
> name_fncs[] =
{
  { "get_val",  [](CiBit& a) { a.get_val();         },  },
  { "set_val",  [](CiBit& a) { a.set_val(rand()%2); },  },
  { "read",     [](CiBit& a) { a.read();            },  },
  { "write",    [](CiBit& a) { a.write();           },  },
  { "encrypt",  [](CiBit& a) { a.encrypt();         },  },
  { "decrypt",  [](CiBit& a) { a.decrypt();         },  },
  { "is_plain", [](CiBit& a) { a.is_plain();        },  },

  { "", nullptr }
};

TEST(CiBit, name_unchanged) {
  for (int idx = 0; ; ++idx)
  {
    auto& op_name = get<0>(name_fncs[idx]);
    auto& op_func = get<1>(name_fncs[idx]);

    if (op_name.empty()) break;

    string a_name = "A";
    CiBit a(rand()%2, a_name);

    op_func(a);

    ASSERT_EQ(a.get_name(), a_name) << "set_val";
  }
}

TEST(CiBit, name_change) {
  {
    string a_name = "A";
    string b_name = "B";
    CiBit a(rand()%2, a_name);

    a.read(b_name);

    ASSERT_EQ(a.get_name(), b_name) << "set_val";
  }

  {
    string a_name = "A";
    string b_name = "B";
    CiBit a(rand()%2, a_name);

    a.write(b_name);

    ASSERT_EQ(a.get_name(), b_name) << "set_val";
  }
}
