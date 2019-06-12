/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ci_bit_vector.hxx>

using namespace std;
using namespace cingulata;

#define RAND_BITV(v, v_size)                                                   \
  {                                                                            \
    for (int i = 0; i < v_size; ++i) {                                         \
      (v).push_back(CiBit(rand() % 2));                                        \
    }                                                                          \
    ASSERT_EQ(v_size, (v).size());                                             \
  }

#define RAND_CIBITV(v)                                                         \
  {                                                                            \
    unsigned int bv_size = rand() % 128;                                       \
    vector<CiBit> bv;                                                          \
    RAND_BITV(bv, bv_size);                                                    \
    (v) = CiBitVector(bv);                                                     \
    (v).encrypt();                                                             \
    ASSERT_EQ((v).size(), bv_size);                                            \
  }

#define RAND_CIBITV_LEN(v, l)                                                  \
  {                                                                            \
    auto ll = (l);                                                             \
    vector<CiBit> bv;                                                          \
    RAND_BITV(bv, ll);                                                         \
    (v) = CiBitVector(bv);                                                     \
    (v).encrypt();                                                             \
    ASSERT_EQ((v).size(), ll);                                                 \
  }

#define ASSERT_EQ_CIBITV_LEN(v1, v2, l)                                        \
  {                                                                            \
    auto _v1 = (v1);                                                           \
    auto _v2 = (v2);                                                           \
    vector<int> v1_dec(l);                                                     \
    vector<int> v2_dec(l);                                                     \
    for (int i = 0; i < l; ++i) {                                              \
      v1_dec[i] = _v1[i].decrypt();                                            \
      v2_dec[i] = _v2[i].decrypt();                                            \
    }                                                                          \
    ASSERT_THAT(v1_dec, ::testing::ElementsAreArray(v2_dec));                  \
  }

#define ASSERT_EQ_CIBITV(v1, v2)                                               \
  {                                                                            \
    ASSERT_EQ((v1).size(), (v2).size());                                       \
    ASSERT_EQ_CIBITV_LEN(v1, v2, (v1).size());                                 \
  }

#define ASSERT_EQ_CIBITV_BITVAL(v, val)                                        \
  {                                                                            \
    auto _v = (v);                                                             \
    auto _val = (val);                                                         \
    vector<int> val_dec(_v.size());                                            \
    vector<int> v_dec(_v.size());                                              \
    for (int i = 0; i < _v.size(); ++i) {                                      \
      val_dec[i] = _val & 1;                                                   \
      v_dec[i] = _v[i].decrypt();                                              \
    }                                                                          \
    ASSERT_THAT(val_dec, ::testing::ElementsAreArray(v_dec));                  \
  }

TEST(CiBitVector, constructor_from_ci_bit) {
  CiBitVector v;
  ASSERT_EQ(v.size(), 0);

  v = CiBitVector(1, CiBit::one);
  ASSERT_EQ(v.size(), 1);
  ASSERT_EQ(v[0].decrypt(), 1);
  ASSERT_EQ(v[0].decrypt(), 1);

  unsigned int v_size = rand() % 128;
  CiBit b(rand() % 2);
  v = CiBitVector(v_size, b);
  ASSERT_EQ(v.size(), v_size);
  ASSERT_EQ_CIBITV_BITVAL(v, b.decrypt());
}

TEST(CiBitVector, constructor_from_list_initializer) {
  vector<CiBit> v1;
  for (unsigned int i = 0; i < 8; ++i) {
    v1.push_back(CiBit(rand() % 2));
  }
  ASSERT_EQ(v1.size(), 8);

  CiBitVector v2 = vector<CiBit>({v1[0].get_val(), v1[1].get_val(), v1[2].get_val(), v1[3].get_val(),
      v1[4].get_val(), v1[5].get_val(), v1[6].get_val(), v1[7].get_val()});

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
  RAND_CIBITV_LEN(v1, rand()%128+1);
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
  RAND_CIBITV_LEN(v1, rand()%128+2);

  unsigned int idx1 = rand() % ((v1.size())/2);
  unsigned int idx2 = 2 * idx1;

  CiBitVector v2 = v1.slice(idx1, idx2);
  ASSERT_EQ(v2.size(), (idx2 - idx1));
  for (unsigned int i = 0; i < v2.size(); ++i) {
    ASSERT_EQ(v2[i].decrypt(), v1[idx1+i].decrypt());
  }

  v2 = v1.slice(idx2, idx1, -1);
  ASSERT_EQ(v2.size(), (idx2 - idx1));
  for (unsigned int i = 0; i < v2.size(); ++i) {
    ASSERT_EQ(v2[i].decrypt(), v1[idx2-i].decrypt());
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
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
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
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
  }
}

TEST(CiBitVector, xor_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2 ^= v2;
  ASSERT_EQ(v2.size(), v1.size());
  ASSERT_EQ_CIBITV_BITVAL(v2, 0);

  v2 = v1;
  CiBitVector v3;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 ^= v3;
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() ^ v3[i].decrypt());
  }
}

TEST(CiBitVector, not_operand) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = v1;
  v2.op_not();
  ASSERT_EQ(v2.size(), v1.size());
  for (unsigned int i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i].decrypt(), (1 - v1[i].decrypt()));
  }
}

TEST(CiBitVector, op_and) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1,rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_and(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 0);
  }

  v2 = v1;
  v2 = v2.op_and(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_and(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
}

TEST(CiBitVector, op_nand) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_nand(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() & v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), 1);
  }

  v2 = v1;
  v2 = v2.op_nand(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() & v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), 1 - v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_nand(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() & v3[i].decrypt()));
}

TEST(CiBitVector, op_or) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_or(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_or(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 1);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_or(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
}

TEST(CiBitVector, op_nor) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_nor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() | v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), 1 - v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_nor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() | v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), 0);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_nor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() | v3[i].decrypt()));
}

TEST(CiBitVector, op_xor) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_xor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() ^ v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_xor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() ^ v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 1 - v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_xor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() ^ v3[i].decrypt());
}

TEST(CiBitVector, op_xnor) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_xnor(v3);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() ^ v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), 1 - v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_xnor(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() ^ v3[i].decrypt()));
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_xnor(v3);
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), 1 - (v1[i].decrypt() ^ v3[i].decrypt()));
}

TEST(CiBitVector, op_andny) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_andny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 0);
  }

  v2 = v1;
  v2 = v2.op_andny(v3, CiBit::one);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_andny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
}

TEST(CiBitVector, op_andyn) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);
  CiBitVector v4 = v3;

  v2 = v2.op_andyn(v3);
  v4.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v4[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_andyn(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v4[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 0);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_andyn(v3);
  v3.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() & v3[i].decrypt());
}

TEST(CiBitVector, op_orny) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);

  v2 = v2.op_orny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  v2 = v2.op_orny(v3, CiBit::one);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 1);
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_orny(v3);
  v1.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
}

TEST(CiBitVector, op_oryn) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  CiBitVector v3;
  unsigned int v3_size = rand() % v2.size();
  RAND_CIBITV_LEN(v3, v3_size);
  CiBitVector v4 = v3;

  v2 = v2.op_oryn(v3);
  v4.op_not();
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v4[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), 1);
  }

  v2 = v1;
  v2 = v2.op_oryn(v3, CiBit::one);
  for (unsigned int i = 0; i < v2.size(); i++) {
    if (i < v3_size)
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v4[i].decrypt());
    else
      ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt());
  }

  v2 = v1;
  RAND_CIBITV_LEN(v3, v2.size());
  v2 = v2.op_oryn(v3);
  v3.op_not();
  for (unsigned int i = 0; i < v2.size(); i++)
    ASSERT_EQ(v2[i].decrypt(), v1[i].decrypt() | v3[i].decrypt());
}

TEST(CiBitVector, rol) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.rol(delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v1[i].decrypt(), v2[(i + delta) % v1.size()].decrypt());
  }

  v1 = v2;
  v1.rol(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v2[i].decrypt(), v1[(i + delta) % v1.size()].decrypt());
  }
}

TEST(CiBitVector, ror) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.ror(delta);
  for (int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v2[i].decrypt(), v1[(i + delta) % v1.size()].decrypt());
  }

  v1 = v2;
  v1.ror(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    ASSERT_EQ(v1[i].decrypt(), v2[(i + delta) % v1.size()].decrypt());
  }
}

TEST(CiBitVector, shl) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.shl(delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].decrypt(), v2[i + delta].decrypt());
    else
      ASSERT_EQ(v1[i].decrypt(), 0);
  }

  v1 = v2;
  v1.shl(delta, CiBit::one);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].decrypt(), v2[i + delta].decrypt());
    else
      ASSERT_EQ(v1[i].decrypt(), 1);
  }

  v1 = v2;
  v1.shl(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].decrypt(), 0);
    else
      ASSERT_EQ(v1[i].decrypt(), v2[i - delta].decrypt());
  }
}

TEST(CiBitVector, shr) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1.shr(delta);
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].decrypt(), 0);
    else
      ASSERT_EQ(v1[i].decrypt(), v2[i - delta].decrypt());
  }

  v1 = v2;
  v1.shr(delta, CiBit::one);
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].decrypt(), 1);
    else
      ASSERT_EQ(v1[i].decrypt(), v2[i - delta].decrypt());
  }

  v1 = v2;
  v1.shr(-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].decrypt(), v2[i + delta].decrypt());
    else
      ASSERT_EQ(v1[i].decrypt(), 0);
  }
}

TEST(CiBitVector, left_shift) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1 <<= delta;
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].decrypt(), v2[i + delta].decrypt());
    else
      ASSERT_EQ(v1[i].decrypt(), 0);
  }

  v1 = v2;
  v1 <<= (-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].decrypt(), 0);
    else
      ASSERT_EQ(v1[i].decrypt(), v2[i - delta].decrypt());
  }
}

TEST(CiBitVector, right_shift) {
  CiBitVector v1;
  RAND_CIBITV_LEN(v1, rand()%128+1);
  CiBitVector v2 = v1;
  int delta = rand() % v1.size();

  v1 >>= delta;
  for (int i = 0; i < v1.size(); i++) {
    if (i < delta)
      ASSERT_EQ(v1[i].decrypt(), 0);
    else
      ASSERT_EQ(v1[i].decrypt(), v2[i - delta].decrypt());
  }

  v1 = v2;
  v1 >>= (-delta);
  for (unsigned int i = 0; i < v1.size(); i++) {
    if (i < (v1.size() - delta))
      ASSERT_EQ(v1[i].decrypt(), v2[i + delta].decrypt());
    else
      ASSERT_EQ(v1[i].decrypt(), 0);
  }
}

TEST(CiBitVector, tilde) {
  CiBitVector v1;
  RAND_CIBITV(v1);
  CiBitVector v2 = ~v1;

  v1.op_not();
  ASSERT_EQ_CIBITV(v2, v1);
}

