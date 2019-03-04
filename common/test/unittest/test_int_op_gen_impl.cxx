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

