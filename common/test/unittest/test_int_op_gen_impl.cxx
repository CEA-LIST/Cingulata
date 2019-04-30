#include <int_op_gen/impl/all.hxx>
#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace cingulata;
using namespace cingulata::int_ops;
using namespace std;

template<typename T>
T to_binary(unsigned val, int n) {
  T res(n,0);
  for (int i = 0; i < n; ++i) {
    res[i] = (val >> i) & 1;
  }
  return res;
}

#define mod(val, bc) ((val) & (((bc) < 64) ? ((1UL<<(bc)) - 1) : -1L))

#define GEN_RAND_BV(VAR, n, val)                                               \
  unsigned VAR##_int = mod(val, n);                                            \
  CiBitVector VAR##_bv = to_binary<CiBitVector>((VAR##_int), (n)).encrypt();

#define ASSERT_EQ_BV_INT(ct_vec1, pt_int)                                      \
  {                                                                            \
    CiBitVector ct_vec = (ct_vec1);                                            \
    ct_vec.decrypt();                                                          \
    auto pt_vec = to_binary<vector<int>>((pt_int), ct_vec.size());             \
    vector<int> ct_vec_dec;                                                    \
    for (int i = 0; i < ct_vec.size(); ++i)                                    \
      ct_vec_dec.emplace_back(ct_vec[i].get_val());                            \
    ASSERT_THAT(ct_vec_dec, ::testing::ElementsAreArray(pt_vec));              \
  }

TEST(IntOpGen, Decoder) {
  const unsigned n = rand() % 12 + 1;

  GEN_RAND_BV(inp, n, rand());

  CiBitVector out = Decoder()(inp_bv);

  /* inputs did not changed */
  ASSERT_EQ_BV_INT(inp_bv, inp_int);

  /* output is valid */
  ASSERT_EQ(out.size(), (1<<n)) << n;
  out.decrypt();
  for (unsigned i = 0; i < out.size(); ++i) {
    if (i == inp_int)
      ASSERT_TRUE(out[i].get_val());
    else
      ASSERT_FALSE(out[i].get_val());
  }
}

TEST(IntOpGen, Mux) {
  const unsigned logn = (rand()%8)+1;
  const unsigned n = 1 << logn;
  const unsigned m = rand() % 32;

  vector<unsigned> vals_m_int;
  vector<CiBitVector> vals_m_bv;

  for (unsigned i = 0; i < n; ++i) {
    GEN_RAND_BV(tmp, m, rand());
    vals_m_int.push_back(tmp_int);
    vals_m_bv.push_back(tmp_bv);
  }

  GEN_RAND_BV(cond, logn, rand());

  CiBitVector out = MuxDepth()(cond_bv, vals_m_bv);

  /* inputs did not changed */
  ASSERT_EQ_BV_INT(cond_bv, cond_int);
  for (int i = 0; i < n; ++i) {
    ASSERT_EQ_BV_INT(vals_m_bv[i], vals_m_int[i]);
  }

  /* output is valid */
  ASSERT_EQ_BV_INT(out, vals_m_int[cond_int]);
}

TEST(IntOpGen, Sort_same) {
  const unsigned size_array = ((rand()%10)+1);
  const unsigned m = ((rand()%16)+1);
  const bool r = ((rand()%2));
  vector<unsigned> vals_int;
  vector<CiBitVector> vals_bv;
  for (unsigned i = 0; i < size_array; ++i) {
    GEN_RAND_BV(tmp, m, rand());
    vals_int.push_back(tmp_int);
    vals_bv.push_back(tmp_bv);
  }

  for (int i = 0; i < size_array; ++i) {
    ASSERT_EQ_BV_INT(vals_bv[i], vals_int[i]);
  }
  if (r == 0)
    sort(vals_int.begin(), vals_int.end());
  else
    sort(vals_int.begin(), vals_int.end(), greater<int>());

  vector<CiBitVector> out = SortDepth(LowerCompSize(), EqualSize(), SklanskyAdder())(vals_bv, r);
  for (int i = 0; i < size_array; ++i) {
    ASSERT_EQ_BV_INT(out[i], vals_int[i]);
  }
}

/*-------------------------------------------------------------------------*/
/**
 * Multiple implementations operators
 */
/*-------------------------------------------------------------------------*/

template<typename T>
string get_oper_name(const testing::TestParamInfo<T>& oper) {
  return get<0>(oper.param);
}
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
 * Binary operator tests
 */
/*-------------------------------------------------------------------------*/

using BinaryParam =
tuple<
  string,
  function<BinaryOper::signature>,
  function<unsigned (const unsigned, const unsigned)>
>;

class Binary : public ::testing::TestWithParam<BinaryParam> {
public:
  string name;
  function<BinaryOper::signature> op_ct;
  function<unsigned (const unsigned, const unsigned)> op_pt;

  virtual void SetUp() {
    auto tmp = GetParam();
    name = get<0>(tmp);
    op_ct = get<1>(tmp);
    op_pt = get<2>(tmp);
  }
};

#define TEST_BINARY_OP(a_int, a_bv, b_int, b_bv)  \
{                                                             \
  auto r_int = op_pt(a_int, b_int);                           \
  auto r_bv = op_ct(a_bv, b_bv);                              \
  /* inputs did not changed */                                \
  ASSERT_EQ_BV_INT(a_bv, a_int);                              \
  ASSERT_EQ_BV_INT(b_bv, b_int);                              \
  /* output is valid */                                       \
  ASSERT_EQ_BV_INT(r_bv, r_int);                              \
}

TEST_P(Binary, random_inps) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_BINARY_OP(a_int, a_bv, b_int, b_bv);
  TEST_BINARY_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Binary, zero_and_randomom_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, 0);

  TEST_BINARY_OP(a_int, a_bv, b_int, b_bv);
  TEST_BINARY_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Binary, 1bit_randomom_inps) {
  const unsigned n = 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_BINARY_OP(a_int, a_bv, b_int, b_bv);
  TEST_BINARY_OP(b_int, b_bv, a_int, a_bv);
}

BinaryParam binary_params[] =
{
  {
    "RippleCarryAdder",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBitVector {
      return RippleCarryAdder()(a,b);
    },
    [](const unsigned a, const unsigned b) -> unsigned {
      return a + b;
    }
  },
  {
    "SklanskyAdder",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBitVector {
      return SklanskyAdder()(a,b);
    },
    [](const unsigned a, const unsigned b) -> unsigned {
      return a + b;
    }
  },
  {
    "WallaceMultiplier",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBitVector {
      return WallaceMultiplier(SklanskyAdder())(a,b);
    },
    [](const unsigned a, const unsigned b) -> unsigned {
      return a * b;
    }
  }
};

INSTANTIATE_TEST_CASE_P(
  IntOpGen,
  Binary,
  ::testing::ValuesIn(binary_params),
  get_oper_name<BinaryParam>
);


/*-------------------------------------------------------------------------*/
/**
 * Comparator tests
 */
/*-------------------------------------------------------------------------*/

using ComparatorParam =
tuple<
  string,
  function<CompOper::signature>,
  function<bool (const unsigned, const unsigned)>
>;

class Comparator : public ::testing::TestWithParam<ComparatorParam>
{
public:
  string name;
  function<CompOper::signature> cmp_ct;
  function<bool (const unsigned, const unsigned)> cmp_pt;

  virtual void SetUp() {
    auto cmp = GetParam();
    name = get<0>(cmp);
    cmp_ct = get<1>(cmp);
    cmp_pt = get<2>(cmp);
  }
};

#define TEST_COMP_OP(a_int, a_bv, b_int, b_bv)                                 \
  {                                                                            \
    auto r_pt = cmp_pt(a_int, b_int);                                          \
    auto r_ct = cmp_ct(a_bv, b_bv);                                            \
    /* inputs did not changed */                                               \
    ASSERT_EQ_BV_INT(a_bv, a_int);                                             \
    ASSERT_EQ_BV_INT(b_bv, b_int);                                             \
    /* output is valid */                                                      \
    ASSERT_EQ(r_ct.decrypt(), r_pt);                                           \
  }

TEST_P(Comparator, random_inps) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_COMP_OP(a_int, a_bv, b_int, b_bv);
  TEST_COMP_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Comparator, zero_and_randomom_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, 0);

  TEST_COMP_OP(a_int, a_bv, b_int, b_bv);
  TEST_COMP_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Comparator, 1bit_randomom_inps) {
  const unsigned n = 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_COMP_OP(a_int, a_bv, b_int, b_bv);
  TEST_COMP_OP(b_int, b_bv, a_int, a_bv);
}

ComparatorParam comp_params[] =
{
  {
    "EqualDepth",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBit {
      return EqualDepth()(a,b);
    },
    [](const unsigned a, const unsigned b) -> bool {
      return a == b;
    }
  },
  {
    "EqualSize",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBit {
      return EqualSize()(a,b);
    },
    [](const unsigned a, const unsigned b) -> bool {
      return a == b;
    }
  },
  {
    "LowerCompDepth",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBit {
      return LowerCompDepth(EqualSize())(a,b);
    },
    [](const unsigned a, const unsigned b) -> bool {
      return a < b;
    }
  },
  {
    "LowerCompSize",
    [](const CiBitVector& a, const CiBitVector& b) -> CiBit {
      return LowerCompSize()(a,b);
    },
    [](const unsigned a, const unsigned b) -> bool {
      return a < b;
    }
  }
};

INSTANTIATE_TEST_CASE_P(
  IntOpGen,
  Comparator,
  ::testing::ValuesIn(comp_params),
  get_oper_name<ComparatorParam>
);


/*-------------------------------------------------------------------------*/
/**
 * Unary operator tests
 */
/*-------------------------------------------------------------------------*/

using UnaryParam =
tuple<
  string,
  function<UnaryOper::signature>,
  function<unsigned (const unsigned)>
>;

class Unary : public ::testing::TestWithParam<UnaryParam> {
public:
  string name;
  function<UnaryOper::signature> op_ct;
  function<unsigned (const unsigned)> op_pt;

  virtual void SetUp() {
    auto tmp = GetParam();
    name = get<0>(tmp);
    op_ct = get<1>(tmp);
    op_pt = get<2>(tmp);
  }
};

#define TEST_UNARY_OP(a_int, a_bv) \
{                                   \
  auto r_int = op_pt(a_int);        \
  auto r_bv = op_ct(a_bv);          \
  /* inputs did not changed */      \
  ASSERT_EQ_BV_INT(a_bv, a_int);    \
  /* output is valid */             \
  ASSERT_EQ_BV_INT(r_bv, r_int);    \
}

TEST_P(Unary, random_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());

  TEST_UNARY_OP(a_int, a_bv);
}

TEST_P(Unary, zero_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, 0);

  TEST_UNARY_OP(a_int, a_bv);
}

TEST_P(Unary, 1bit_randomom_inp) {
  const unsigned n = 1;

  GEN_RAND_BV(a, n, rand());

  TEST_UNARY_OP(a_int, a_bv);
}

UnaryParam unary_params[] =
{
  {
    "Negate",
    [](const CiBitVector& a) -> CiBitVector {
      return Negate(RippleCarryAdder())(a);
    },
    [](const unsigned a) -> unsigned {
      return -a;
    }
  }
};

INSTANTIATE_TEST_CASE_P(
  IntOpGen,
  Unary,
  ::testing::ValuesIn(unary_params),
  get_oper_name<UnaryParam>
);

void test_multi_input_adder(const unsigned n, const unsigned m) {
  const unsigned logn = (unsigned)ceil(log2(n));

  vector<unsigned> inps_int;
  unsigned out_int = 0;
  unsigned max_out_int = 0;
  vector<CiBitVector> inps_bv;
  for (unsigned i = 0; i < n; ++i) {
    GEN_RAND_BV(tmp, m, rand());
    tmp_int = mod(tmp_int, tmp_bv.size());
    inps_int.push_back(tmp_int);
    inps_bv.push_back(tmp_bv);
    out_int += tmp_int;
    max_out_int += (1<<tmp_bv.size())-1;
  }

  CiBitVector out_bv = MultiInputAdder(RippleCarryAdder())(inps_bv);

  /* inputs did not changed */
  for (int i = 0; i < n; ++i) {
    ASSERT_EQ_BV_INT(inps_bv[i], inps_int[i]);
  }

  /* output is valid */
  ASSERT_EQ_BV_INT(out_bv, out_int);
  int out_size = (int)ceil(log2(max_out_int+1));
  ASSERT_EQ(out_bv.size(), out_size) << " n: " << n << " logn: " << logn << " m: " << m;
}

TEST(MultiInputAdder, same_size_single) {
  test_multi_input_adder(0, 0);
  test_multi_input_adder(0, 5);
  test_multi_input_adder(7, 0);
  test_multi_input_adder(8, 1);
  test_multi_input_adder(8, 7);
  test_multi_input_adder(8, 14);
  test_multi_input_adder(7, 1);
  test_multi_input_adder(7, 7);
  test_multi_input_adder(7, 14);
}

TEST(MultiInputAdder, same_size) {
  test_multi_input_adder((rand() % 256) + 1 , (rand() % 16) + 1);
}

TEST(MultiInputAdder, diff_size) {
  const unsigned n = (rand() % 256) +1;
  const unsigned logn = (unsigned)ceil(log2(n));

  vector<unsigned> inps_int;
  unsigned out_int = 0;
  unsigned max_out_int = 0;
  vector<CiBitVector> inps_bv;
  for (unsigned i = 0; i < n; ++i) {
    GEN_RAND_BV(tmp, (rand() % 16) + 1, rand());
    tmp_int = mod(tmp_int, tmp_bv.size());
    inps_int.push_back(tmp_int);
    inps_bv.push_back(tmp_bv);
    out_int += tmp_int;
    max_out_int += (1<<tmp_bv.size())-1;
  }

  CiBitVector out_bv = MultiInputAdder(RippleCarryAdder())(inps_bv);

  /* inputs did not change */
  for (int i = 0; i < n; ++i) {
    ASSERT_EQ_BV_INT(inps_bv[i], inps_int[i]);
  }

  /* output is valid */
  ASSERT_EQ_BV_INT(out_bv, out_int);
  int out_size = (int)ceil(log2(max_out_int+1));
  ASSERT_EQ(out_bv.size(), out_size) << " n: " << n << " logn: " << logn;

}
