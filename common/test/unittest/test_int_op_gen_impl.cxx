#include <int_op_gen/impl/all.hxx>

#include <gtest/gtest.h>

using namespace cingulata;
using namespace cingulata::int_ops;
using namespace std;

#define ASSERT_EQ_CT_PT_VEC(ct_vec, pt_vec)       \
ASSERT_EQ((ct_vec).size(), (pt_vec).size());      \
for (int i = 0; i < (ct_vec).size(); ++i) {       \
  ASSERT_EQ((ct_vec)[i].get_val(), (pt_vec)[i]);  \
}

#define ASSERT_EQ_CT_PT_MAT(ct_mat, pt_mat)               \
ASSERT_EQ((ct_mat).size(), (pt_mat).size());              \
for (int i = 0; i < (ct_mat).size(); ++i) {               \
  ASSERT_EQ((ct_mat)[i].size(), (pt_mat)[i].size());      \
  for (int j = 0; j < (ct_mat)[i].size(); ++j) {          \
    ASSERT_EQ((ct_mat)[i][j].get_val(), (pt_mat)[i][j]);  \
  }                                                       \
}

template<typename T>
T to_binary(unsigned val, int n) {
  T res(n,0);
  for (int i = 0; i < n; ++i) {
    res[i] = (val >> i) & 1;
  }
  return res;
}

#define mod(val, bc) ((val) & (((bc) < 64) ? ((1UL<<(bc)) - 1) : -1L))

#define GEN_RAND_BV(VAR, n, val)                               \
  unsigned VAR ## _int = mod(val, n);                           \
  vector<int> VAR ## _pv = to_binary<vector<int>>((VAR ## _int), (n));  \
  CiBitVector VAR ## _bv = to_binary<CiBitVector>((VAR ## _int), (n));

#define ASSERT_EQ_BV_INT(ct_vec, pt_int)            \
{                                                   \
  auto pt_vec = to_binary<vector<int>>((pt_int),    \
    (ct_vec).size());                               \
  for (int i = 0; i < (ct_vec).size(); ++i) {       \
    ASSERT_EQ((ct_vec)[i].get_val(), pt_vec[i]);    \
  }                                                 \
}


TEST(IntOpGen, Decoder) {
  vector<char> inp_vec(rand()%12+1, 0);
  unsigned inp_int = 0;
  CiBitVector inp(inp_vec.size(), 0);

  for (unsigned i = 0; i < inp.size(); ++i) {
    if (rand() % 2) {
      inp_vec[i] = 1;
      inp_int |= 1 << i;
      inp[i] = 1;
    }
  }

  CiBitVector out = Decoder()(inp);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(inp, inp_vec);

  /* output is valid */
  ASSERT_EQ(out.size(), (1<<inp.size()));
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
  const unsigned m = rand() % 48;

  vector<vector<int>> vals_pt;
  vector<CiBitVector> vals;
  for (unsigned i = 0; i < n; ++i) {
    vals_pt.push_back(vector<int>(m,0));
    vals.push_back(CiBitVector(m,0));
    for (unsigned j = 0; j < m; ++j) {
      if (rand()%2) {
        vals[i][j] = 1;
        vals_pt[i][j] = 1;
      }
    }
  }

  vector<int> cond_pt(logn, 0);
  unsigned cond_idx = 0;
  CiBitVector cond(cond_pt.size(), 0);
  for (unsigned i = 0; i < logn; ++i) {
    if (rand() % 2) {
      cond_pt[i] = 1;
      cond[i] = 1;
      cond_idx |= 1 << i;
    }
  }

  CiBitVector out = MuxDepth()(cond, vals);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(cond, cond_pt);
  ASSERT_EQ_CT_PT_MAT(vals, vals_pt);

  /* output is valid */
  ASSERT_EQ_CT_PT_VEC(out, vals_pt[cond_idx]);
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

TEST_P(Binary, non_zero_inps) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_BINARY_OP(a_int, a_bv, b_int, b_bv);
  TEST_BINARY_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Binary, one_zero_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, 0);

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

#define TEST_COMP_OP(a_int, a_bv, b_int, b_bv)  \
{                                               \
  auto r_pt = cmp_pt(a_int, b_int);             \
  auto r_ct = cmp_ct(a_bv, b_bv);               \
  /* inputs did not changed */                  \
  ASSERT_EQ_BV_INT(a_bv, a_int);                \
  ASSERT_EQ_BV_INT(b_bv, b_int);                \
  /* output is valid */                         \
  ASSERT_EQ(r_ct.get_val(), r_pt);              \
}

TEST_P(Comparator, non_zero_inps) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, rand());

  TEST_COMP_OP(a_int, a_bv, b_int, b_bv);
  TEST_COMP_OP(b_int, b_bv, a_int, a_bv);
}

TEST_P(Comparator, one_zero_inp) {
  const unsigned n = rand() % 32 + 1;

  GEN_RAND_BV(a, n, rand());
  GEN_RAND_BV(b, n, 0);

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
