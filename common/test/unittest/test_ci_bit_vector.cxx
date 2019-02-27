#include <gtest/gtest.h>

#include <ci_bit_vector.hxx>

using namespace std;
using namespace cingulata;

#define RAND_BITV(v, v_size)          \
{                                     \
  for (int i = 0; i < v_size; ++i) {  \
    (v).push_back(CiBit(rand() % 2)); \
  }                                   \
  ASSERT_EQ(v_size, (v).size());      \
}                                     \

#define RAND_CIBITV(v)                               \
{                                                    \
  unsigned int bv_size = rand() % 128;               \
  vector<CiBit> bv;                                  \
  RAND_BITV(bv, bv_size);                            \
  (v) = CiBitVector(bv);                             \
  ASSERT_EQ((v).size(), bv_size);                    \
}                                                    \

#define RAND_CIBITV_LEN(v, l)                        \
{                                                    \
  vector<CiBit> bv;                                  \
  RAND_BITV(bv, (l));                                \
  (v) = CiBitVector(bv);                             \
  ASSERT_EQ((v).size(), (l));                        \
}                                                    \

#define ASSERT_EQ_CIBITV(v1, v2)                      \
{                                                     \
  ASSERT_EQ((v1).size(), (v2).size());                \
  for (int i = 0; i < (v1).size(); ++i) {             \
    ASSERT_EQ((v1)[i].get_val(), (v2)[i].get_val());  \
  }                                                   \
}                                                     \

#define ASSERT_EQ_CIBITV_LEN(v1, v2, l)               \
{                                                     \
  for (int i = 0; i < (l); ++i) {                     \
    ASSERT_EQ((v1)[i].get_val(), (v2)[i].get_val());  \
  }                                                   \
}                                                     \

#define ASSERT_EQ_CIBITV_INTVAL(v, val)               \
{                                                     \
  for (int i = 0; i < (v).size(); ++i) {              \
    ASSERT_EQ((v)[i].get_val(), (val));               \
  }                                                   \
}                                                     \


TEST(CiBitVector, constructor_from_ci_bit) {
  CiBitVector v;
  ASSERT_EQ(v.size(), 1);
  ASSERT_EQ(v[0].get_val(), CiBit::zero.get_val());
  ASSERT_EQ(v[0].get_val(), 0);

  v = CiBitVector(1, CiBit::one);
  ASSERT_EQ(v.size(), 1);
  ASSERT_EQ(v[0].get_val(), CiBit::one.get_val());
  ASSERT_EQ(v[0].get_val(), 1);

  unsigned int v_size = rand() % 128;
  CiBit b(rand() % 2);
  v = CiBitVector(v_size, b);
  ASSERT_EQ(v.size(), v_size);
  ASSERT_EQ_CIBITV_INTVAL(v, b.get_val());
}

TEST(CiBitVector, constructor_from_list_initializer) {
  vector<CiBit> v1;
  for (unsigned int i = 0; i < 8; ++i) {
    v1.push_back(CiBit(rand() % 2));
  }
  ASSERT_EQ(v1.size(), 8);

  CiBitVector v2 {v1[0].get_val(), v1[1].get_val(), v1[2].get_val(), v1[3].get_val(),
    v1[4].get_val(), v1[5].get_val(), v1[6].get_val(), v1[7].get_val()};

  ASSERT_EQ(v1.size(), v2.size());
  for (unsigned int i = 0; i < 8; ++i) {
    ASSERT_EQ(v1[i].get_val(), v2[i].get_val());
  }
}

TEST(CiBitVector, constructor_from_ci_bit_vector) {
  unsigned int v1_size = rand() % 128;
  vector<CiBit> v1;
  RAND_BITV(v1, v1_size);

  CiBitVector v2(v1);
  ASSERT_EQ(v1_size, v2.size());
  for (unsigned int j = 0; j < v1_size; ++j) {
    ASSERT_EQ(v1[j].get_val(), v2[j].get_val());
  }
}

TEST(CiBitVector, copy_constructor) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2(v1);
  ASSERT_EQ_CIBITV(v1, v2);
}

TEST(CiBitVector, assign_from_ci_bit_vector) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector  v2 = v1;
  ASSERT_EQ_CIBITV(v1, v2);
}

TEST(CiBitVector, resize) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2(v1);
  unsigned int delta = rand() % v2.size();

  v1.resize(v2.size() - delta);
  ASSERT_EQ(v1.size(), v2.size() - delta);
  ASSERT_EQ_CIBITV_LEN(v1, v2, v1.size());

  v1 = v2;
  v1.resize(v2.size() + delta, CiBit::one);
  ASSERT_EQ(v1.size(), v2.size() + delta);
  ASSERT_EQ_CIBITV_LEN(v1, v2, v2.size());
  for (unsigned int i = v2.size(); i < v1.size(); ++i) {
    ASSERT_EQ(v1[i].get_val(), 1);
  }
}

TEST(CiBitVector, at) {
  unsigned int v_size = rand() % 128;
  vector<CiBit> v;
  RAND_BITV(v, v_size);
  CiBitVector v1(v);

  for (unsigned int i = 0; i < v_size; ++i) {
    ASSERT_EQ(v1.at(i).get_val(), v[i].get_val());
  }
}

TEST(CiBitVector, slice) {
  CiBitVector v1;
  RAND_CIBITV(v1);

  unsigned int idx1 = rand() % ((v1.size())/2);
  unsigned int idx2 = 2 * idx1;

  CiBitVector v2 = v1.slice(idx1, idx2);
  ASSERT_EQ(v2.size(), (idx2 - idx1));
  for (unsigned int i = 0; i < v2.size(); ++i) {
    ASSERT_EQ(v2[i].get_val(), v1[idx1+i].get_val());
  }

  v2 = v1.slice(idx2, idx1, -1);
  ASSERT_EQ(v2.size(), (idx2 - idx1));
  for (unsigned int i = 0; i < v2.size(); ++i) {
    ASSERT_EQ(v2[i].get_val(), v1[idx2-i].get_val());
  }
}

TEST(CiBitVector, and_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2 &= v2;
  ASSERT_EQ(v2.size(), v1.size());
  ASSERT_EQ_CIBITV(v2, v1);

  CiBitVector v3;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 &= v3;
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
  }
}

TEST(CiBitVector, or_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2 |= v2;
  ASSERT_EQ(v2.size(), v1.size());
  ASSERT_EQ_CIBITV(v2, v1);

  CiBitVector v3;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 |= v3;
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
  }
}

TEST(CiBitVector, xor_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2 ^= v2;
  ASSERT_EQ(v2.size(), v1.size());
  ASSERT_EQ_CIBITV_INTVAL(v2, 0);

  v2 = v1;
  CiBitVector v3;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 ^= v3;
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() ^ v3[i].get_val());
  }
}

TEST(CiBitVector, not_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2.op_not();
  ASSERT_EQ(v2.size(), v1.size());
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), (1 - v1[i].get_val()));
  }
}

TEST(CiBitVector, op_and) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_and(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 0);
  }

  v2 = v1;
  v2 = v2.op_and(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_and(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
}

TEST(CiBitVector, op_nand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_nand(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() & v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), 1);
  }

  v2 = v1;
  v2 = v2.op_nand(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() & v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), 1 - v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_nand(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() & v3[i].get_val()));
}

TEST(CiBitVector, op_or) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_or(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_or(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 1);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_or(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
}

TEST(CiBitVector, op_nor) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_nor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() | v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), 1 - v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_nor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() | v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), 0);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_nor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() | v3[i].get_val()));
}

TEST(CiBitVector, op_xor) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_xor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() ^ v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_xor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() ^ v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 1 - v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_xor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() ^ v3[i].get_val());
}

TEST(CiBitVector, op_xnor) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_xnor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() ^ v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), 1 - v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_xnor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() ^ v3[i].get_val()));
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_xnor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), 1 - (v1[i].get_val() ^ v3[i].get_val()));
}

TEST(CiBitVector, op_andny) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_andny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 0);
  }

  v2 = v1;
  v2 = v2.op_andny(v3, CiBit::one);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_andny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
}

TEST(CiBitVector, op_andyn) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);
  CiBitVector v4 = v3;

  v2 = v2.op_andyn(v3);
  v4.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v4[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_andyn(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v4[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 0);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_andyn(v3);
  v3.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() & v3[i].get_val());
}

TEST(CiBitVector, op_orny) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_orny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  v2 = v2.op_orny(v3, CiBit::one);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 1);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_orny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
}

TEST(CiBitVector, op_oryn) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);
  CiBitVector v4 = v3;

  v2 = v2.op_oryn(v3);
  v4.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v4[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), 1);
  }

  v2 = v1;
  v2 = v2.op_oryn(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v4[i].get_val());
    else
      ASSERT_EQ(v2[i].get_val(), v1[i].get_val());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_oryn(v3);
  v3.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].get_val(), v1[i].get_val() | v3[i].get_val());
}

TEST(CiBitVector, rol) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.rol(delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v1[i].get_val(), v2[(i + delta) % v1.size()].get_val());
  }

  v1 = v2;
  v1.rol(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), v1[(i + delta) % v1.size()].get_val());
  }
}

TEST(CiBitVector, ror) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.ror(delta);
  for (int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v2[i].get_val(), v1[(i + delta) % v1.size()].get_val());
  }

  v1 = v2;
  v1.ror(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v1[i].get_val(), v2[(i + delta) % v1.size()].get_val());
  }
}

TEST(CiBitVector, shl) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.shl(delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].get_val(), v2[i + delta].get_val());
    else
      ASSERT_EQ(v1[i].get_val(), 0);
  }

  v1 = v2;
  v1.shl(delta, CiBit::one);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].get_val(), v2[i + delta].get_val());
    else
      ASSERT_EQ(v1[i].get_val(), 1);
  }

  v1 = v2;
  v1.shl(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].get_val(), 0);
    else
      ASSERT_EQ(v1[i].get_val(), v2[i - delta].get_val());
  }
}

TEST(CiBitVector, shr) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.shr(delta);
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].get_val(), 0);
    else
      ASSERT_EQ(v1[i].get_val(), v2[i - delta].get_val());
  }

  v1 = v2;
  v1.shr(delta, CiBit::one);
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].get_val(), 1);
    else
      ASSERT_EQ(v1[i].get_val(), v2[i - delta].get_val());
  }

  v1 = v2;
  v1.shr(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].get_val(), v2[i + delta].get_val());
    else
      ASSERT_EQ(v1[i].get_val(), 0);
  }
}

TEST(CiBitVector, left_shift) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1 <<= delta;
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].get_val(), v2[i + delta].get_val());
    else
      ASSERT_EQ(v1[i].get_val(), 0);
  }

  v1 = v2;
  v1 <<= (-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].get_val(), 0);
    else
      ASSERT_EQ(v1[i].get_val(), v2[i - delta].get_val());
  }
}

TEST(CiBitVector, right_shift) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1 >>= delta;
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].get_val(), 0);
    else
      ASSERT_EQ(v1[i].get_val(), v2[i - delta].get_val());
  }

  v1 = v2;
  v1 >>= (-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].get_val(), v2[i + delta].get_val());
    else
      ASSERT_EQ(v1[i].get_val(), 0);
  }
}

TEST(CiBitVector, tilde) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = ~v1;

  v1.op_not();
  ASSERT_EQ_CIBITV(v2, v1);
}

