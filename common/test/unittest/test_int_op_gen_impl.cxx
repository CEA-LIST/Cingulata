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
 * Adder test
 */
/*-------------------------------------------------------------------------*/

class Adder : public ::testing::TestWithParam<const BinaryOper*> {
public:
  const BinaryOper* adder;
  virtual void SetUp() {
    adder = GetParam();
  }
};

#define GEN_RAND_BV(VAR, n)             \
  vector<char> VAR ## _pv((n), 0);      \
  unsigned VAR ## _int = 0;             \
  CiBitVector VAR ## _bv((n), 0);       \
  for (unsigned i = 0; i < (n); ++i) {  \
    if (rand() % 2) {                   \
      (VAR ## _pv)[i] = 1;              \
      (VAR ## _int)  |= 1 << i;         \
      (VAR ## _bv)[i] = 1;              \
    }                                   \
  }

TEST_P(Adder, non_zero_inps) {
  const unsigned n = rand() % 32 + 1;
  GEN_RAND_BV(a, n);
  GEN_RAND_BV(b, n);

  vector<char> r_pv(n,0);
  unsigned r_int = a_int + b_int;
  for (int i = 0; i < n; ++i) {
    r_pv[i] = (r_int >> i) & 1;
  }

  CiBitVector r_bv = (*adder)(a_bv, b_bv);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(a_bv, a_pv);
  ASSERT_EQ_CT_PT_VEC(b_bv, b_pv);

  /* output is valid */
  ASSERT_EQ_CT_PT_VEC(r_bv, r_pv);
}

TEST_P(Adder, order) {
  const unsigned n = rand() % 32 + 1;
  GEN_RAND_BV(a, n);
  GEN_RAND_BV(b, n);

  vector<char> r_pv(n,0);
  unsigned r_int = a_int + b_int;
  for (int i = 0; i < n; ++i) {
    r_pv[i] = (r_int >> i) & 1;
  }

  CiBitVector r1_bv = (*adder)(a_bv, b_bv);
  CiBitVector r2_bv = (*adder)(b_bv, a_bv);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(a_bv, a_pv);
  ASSERT_EQ_CT_PT_VEC(b_bv, b_pv);

  /* output is valid */
  ASSERT_EQ_CT_PT_VEC(r1_bv, r_pv);
  ASSERT_EQ_CT_PT_VEC(r2_bv, r_pv);
}

TEST_P(Adder, one_zero_inp) {
  const unsigned n = rand() % 32 + 1;
  GEN_RAND_BV(a, n);

  const CiBitVector b_bv(n, 0);
  const vector<char> b_pv(n, 0);

  CiBitVector r_bv = (*adder)(a_bv, b_bv);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(a_bv, a_pv);
  ASSERT_EQ_CT_PT_VEC(b_bv, b_pv);

  /* output is valid */
  ASSERT_EQ_CT_PT_VEC(r_bv, a_pv);

  /* change direction */
  r_bv = (*adder)(b_bv, a_bv);

  /* inputs did not changed */
  ASSERT_EQ_CT_PT_VEC(a_bv, a_pv);
  ASSERT_EQ_CT_PT_VEC(b_bv, b_pv);

  /* output is valid */
  ASSERT_EQ_CT_PT_VEC(r_bv, a_pv);
}

string adder_name(const testing::TestParamInfo<const BinaryOper*>& adder) {
  if (dynamic_cast<const RippleCarryAdder*>(adder.param))
    return "RippleCarry";
  else if (dynamic_cast<const SklanskyAdder*>(adder.param))
    return "Sklansky";
  else
    return "UNKNOWN";
}
const BinaryOper* adder_impls[] = {
  new RippleCarryAdder(),
  new SklanskyAdder()
};

INSTANTIATE_TEST_CASE_P(IntOpGen, Adder, ::testing::ValuesIn(adder_impls), adder_name);
