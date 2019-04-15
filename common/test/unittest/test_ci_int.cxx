#include <ci_context.hxx>
#include <ci_int.hxx>

#include <gtest/gtest.h>

using namespace std;
using namespace cingulata;

/**
 * @brief      Signed int random
 */
#define irand() (rand() | ((rand()%2) << 31))

/**
 * @brief      Signed long random
 */
#define lrand() (((long)irand() << 32) + (long)irand())

/**
 * @brief      Random integer in range [a, b)
 */
#define rrand(a, b) (a + rand() % (b-a))

/**
 * @brief      Modulo val to bc bits
 */
#define mod(val, bc) ((val) & (((bc) < 64) ? ((1UL<<(bc)) - 1) : -1L))

#define GEN_RAND_CI_L(VAR, size, is_signed)                 \
  long VAR ## _val = lrand();                               \
  unsigned VAR ## _size = (size);                           \
  bool VAR ## _is_signed = (is_signed);                     \
  if (VAR ## _is_signed and VAR ## _size != 0)              \
    VAR ## _val = VAR ## _val >> (64 - VAR ## _size);       \
  else                                                      \
    VAR ## _val = mod(VAR ## _val, VAR ## _size);           \
  CiInt VAR(VAR ## _val, VAR ## _size, VAR ## _is_signed);


#define ASSERT_CI_PARAM(a, a_size, a_is_signed) \
{                                               \
  ASSERT_EQ((a).size(), a_size);                \
  ASSERT_EQ((a).is_signed(), a_is_signed);      \
}

#define ASSERT_EQ_CI_L(a, b)                      \
{                                                 \
  ASSERT_LE((a).size(), 64);                      \
  for (int i = 0; i < (a).size(); ++i) {          \
    ASSERT_EQ((a)[i].get_val(), ((b) >> i) & 1);  \
  }                                               \
}

#define ASSERT_EQ_CI_CI_S(a, b)                                                \
  {                                                                            \
    auto &aa = (a);                                                            \
    auto &bb = (b);                                                            \
    for (int i = 0; i < aa.size(); ++i) {                                      \
      ASSERT_EQ(aa[i].get_val(), bb[i].get_val());                             \
    }                                                                          \
    for (int i = aa.size(); i < bb.size(); ++i) {                              \
      ASSERT_EQ(aa.sign().get_val(), bb[i].get_val());                         \
    }                                                                          \
  }

#define ASSERT_EQ_CI_CI(a, b)                                                  \
  {                                                                            \
    if ((a).size() < (b).size()) {                                             \
      ASSERT_EQ_CI_CI_S((a), (b))                                              \
    } else {                                                                   \
      ASSERT_EQ_CI_CI_S((b), (a))                                              \
    }                                                                          \
  }

TEST(CiInt, constructor_from_ci_bit) {
  CiBit b(rand() % 2);
  CiInt x(b);
  ASSERT_EQ(x.is_signed(), false);
  ASSERT_EQ(x.size(), 1);
  ASSERT_EQ(x[0].get_val(), b.get_val());
}

TEST(CiInt, constructor_from_ci_bit_vector) {
  int size = rand() % 128;
  vector<CiBit> v;
  for (unsigned int j = 0; j < size; j++) {
    v.push_back(CiBit(rand() % 2));
  }

  CiInt x(v);
  ASSERT_EQ(x.is_signed(), false);
  ASSERT_EQ(x.size(), size);
  for (unsigned int j = 0; j < size; j++) {
    ASSERT_EQ(x[j].get_val(), v[j].get_val());
  }
}

TEST(CiInt, encode_plain_val) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);

  ASSERT_CI_PARAM(x, x_size, x_is_signed);
  ASSERT_EQ_CI_L(x, x_val);

  CiInt z(x_val);
  ASSERT_CI_PARAM(z, sizeof(long)*8, true);
  ASSERT_EQ_CI_L(z, x_val);
}

TEST(CiInt, assign_from_ci_int) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);
  CiInt y = x;

  ASSERT_CI_PARAM(x, x_size, x_is_signed);
  ASSERT_EQ_CI_L(x, x_val);

  ASSERT_CI_PARAM(y, x_size, x_is_signed);
  ASSERT_EQ_CI_L(y, x_val);
}

TEST(CiInt, assign_from_value) {
  {
    int val = rand();
    CiInt y = val;
    ASSERT_CI_PARAM(y, sizeof(int)*8, true);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    unsigned val = rand();
    CiInt y = val;
    ASSERT_CI_PARAM(y, sizeof(unsigned)*8, false);
    ASSERT_EQ_CI_L(y, val);
  }
}

TEST(CiInt, alter) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);

  unsigned y_size = rand() % 64;
  bool y_is_signed = rand() % 2;

  CiInt y{x.alter(y_size, y_is_signed)};

  ASSERT_CI_PARAM(x, x_size, x_is_signed);
  ASSERT_EQ_CI_L(x, x_val);

  ASSERT_CI_PARAM(y, y_size, y_is_signed);

  CiInt xx{x.cast(y_size)};
  ASSERT_EQ_CI_CI(xx, y);
}

TEST(CiInt, is_signed) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);
  ASSERT_EQ(x.is_signed(), x_is_signed);
}

TEST(CiInt, change_sign) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);
  ASSERT_EQ(x.change_sign(true).is_signed(), true);
  ASSERT_EQ(x.change_sign(false).is_signed(), false);
  ASSERT_EQ(x.to_signed().is_signed(), true);
  ASSERT_EQ(x.to_unsigned().is_signed(), false);
}

TEST(CiInt, resize) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);

  unsigned y_size = rand() % 64;
  CiInt y{x.resize(y_size)};

  ASSERT_CI_PARAM(x, x_size, x_is_signed);
  ASSERT_EQ_CI_L(x, x_val);

  ASSERT_CI_PARAM(y, y_size, x_is_signed);

  CiInt xx{x.cast(y_size)};
  ASSERT_EQ_CI_CI(xx, y);
}

TEST(CiInt, lsb) {
  CiInt x(rand(), rand() % 32+1, rand() % 2);
  ASSERT_EQ(x.lsb().get_val(), x[0].get_val());
}

TEST(CiInt, msb) {
  CiInt x(rand(), rand() % 32+1, rand() % 2);
  ASSERT_EQ(x.msb().get_val(), x[x.size()-1].get_val());
}

TEST(CiInt, sign) {
  CiInt x(255, 8, false);
  ASSERT_EQ(x.sign().get_val(), 0);

  CiInt y = CiInt(255, 8, true);
  ASSERT_EQ(y.sign().get_val(), 1);
}

TEST(CiInt, cast) {
  CiInt x(rand());
  CiBitVector y = x.cast();
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }
}

TEST(CiInt, and_operand) {
  CiInt x(rand());
  CiInt y = x;
  y &= y;
  ASSERT_EQ_CI_CI(x, y);

  CiInt z(rand());
  y &= z;
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() & z[i].get_val());
  }
}

TEST(CiInt, or_operand) {
  CiInt x(rand());
  CiInt y = x;
  y |= y;
  ASSERT_EQ_CI_CI(x, y);

  CiInt z(rand());
  y |= z;
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() | z[i].get_val());
  }
}

TEST(CiInt, xor_operand) {
  CiInt x(rand());
  x ^= x;
  ASSERT_EQ_CI_L(x, 0L);

  x = CiInt(rand());
  CiInt y = x;
  CiInt z(rand());
  y ^= z;
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() ^ z[i].get_val());
  }
}

TEST(CiInt, right_shift) {
  CiInt x(rand());
  int delta = rand() % (x.size());
  CiInt y = x;
  x <<= delta;
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    if (i < delta)
      ASSERT_EQ(x[i].get_val(), 0);
    else
      ASSERT_EQ(x[i].get_val(), y[i - delta].get_val());
  }

  CiInt z = y << delta;
  ASSERT_EQ(z.size(), y.size());
  for (unsigned int i = 0; i < z.size(); i++) {
    ASSERT_EQ(x[i].get_val(), z[i].get_val());
  }

  /* right shift with a negative value */
  x = y;
  x <<= (-delta);
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    if (i < (x.size() - delta))
      ASSERT_EQ(x[i].get_val(), y[i + delta].get_val());
    else
      ASSERT_EQ(x[i].get_val(), y.msb().get_val());
  }

  z = y << (-delta);
  ASSERT_EQ(z.size(), y.size());
  for (unsigned int i = 0; i < z.size(); i++) {
    ASSERT_EQ(x[i].get_val(), z[i].get_val());
  }
}

TEST(CiInt, left_shift) {
  CiInt x(rand());
  int delta = rand() % (x.size());
  CiInt y = x;
  x >>= delta;
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    if (i < (x.size() - delta))
      ASSERT_EQ(x[i].get_val(), y[i + delta].get_val());
    else
      ASSERT_EQ(x[i].get_val(), y.msb().get_val());
  }

  CiInt z = y >> delta;
  ASSERT_EQ(z.size(), y.size());
  for (unsigned int i = 0; i < z.size(); i++) {
    ASSERT_EQ(x[i].get_val(), z[i].get_val());
  }

  /* left shift with a negative value */
  x = y;
  x >>= (-delta);
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    if (i < delta)
      ASSERT_EQ(x[i].get_val(), 0);
    else
      ASSERT_EQ(x[i].get_val(), y[i - delta].get_val());
  }

  z = y >> (-delta);
  ASSERT_EQ(z.size(), y.size());
  for (unsigned int i = 0; i < z.size(); i++) {
    ASSERT_EQ(x[i].get_val(), z[i].get_val());
  }
}

TEST(CiInt, rol) {
  CiInt x(rand());
  CiInt y = x;
  unsigned int x_size = x.size();
  int delta = rand() % x_size;

  x.rol(delta);
  ASSERT_EQ(x_size, y.size());
  for (unsigned int i = 0; i < x_size; i++) {
    ASSERT_EQ(y[i].get_val(), x[(i + delta) % x_size].get_val());
  }

  x = y;
  x.rol(-delta);
  ASSERT_EQ(x_size, y.size());
  for (int i = x_size-1; i >= 0; i--) {
    ASSERT_EQ(y[i].get_val(), x[(i - delta) % x_size].get_val());
  }
}

TEST(CiInt, ror) {
  CiInt x(rand());
  CiInt y = x;
  unsigned int x_size = x.size();
  int delta = rand() % x_size;

  x.ror(delta);
  ASSERT_EQ(x_size, y.size());
  for (int i = x_size-1; i >= 0; i--) {
    ASSERT_EQ(y[i].get_val(), x[(i - delta) % x_size].get_val());
  }

  x = y;
  x.ror(-delta);
  ASSERT_EQ(x_size, y.size());
  for (unsigned int i = 0; i < x_size; i++) {
    ASSERT_EQ(y[i].get_val(), x[(i + delta) % x_size].get_val());
  }
}


/**
 * Test operations based on IIntOpGen
 */

#include <int_op_gen/size.hxx>
#include <int_op_gen/mult_depth.hxx>

template<typename IntOpGenT>
class CiInt_OpGen : public ::testing::Test {
protected:
  IntOpGenT* m_op_gen;

  void SetUp() override {
    m_op_gen = new IntOpGenT();
    CiContext::set_int_op_gen(m_op_gen);
  }

  void TearDown() override {
    delete m_op_gen;
    CiContext::set_int_op_gen(nullptr);
  }
};

typedef ::testing::Types
<
  IntOpGenSize,
  IntOpGenDepth
>
IntOpGenTypes;

TYPED_TEST_CASE(CiInt_OpGen, IntOpGenTypes);


/**
 * Test one input arithmetic operators or operators applied on same input
 */
#define OP_GEN_ARITH_1_INP(TEST_NAME, OPER_CT, OPER_PT) \
TYPED_TEST(CiInt_OpGen, TEST_NAME) {                    \
  GEN_RAND_CI_L(x, rand()%32+1, rand()%2);              \
  CiInt r = OPER_CT(x);                                 \
  ASSERT_CI_PARAM(x, x_size, x_is_signed);              \
  ASSERT_EQ_CI_L(x, x_val);                             \
  ASSERT_CI_PARAM(r, x_size, x_is_signed);              \
  ASSERT_EQ_CI_L(r, OPER_PT(x_val));                    \
}

OP_GEN_ARITH_1_INP( plus,
                    [] (const CiInt& a) -> CiInt {return a;},
                    [] (const long& a) -> long {return a;});

OP_GEN_ARITH_1_INP( neg,
                    [] (const CiInt& a) -> CiInt {return -a;},
                    [] (const long& a) -> long {return -a;});

OP_GEN_ARITH_1_INP( add_same_inp,
                    [] (const CiInt& a) -> CiInt {return a+a;},
                    [] (const long& a) -> long {return 2*a;});

OP_GEN_ARITH_1_INP( sub_same_inp,
                    [] (const CiInt& a) -> CiInt {return a-a;},
                    [] (const long& a) -> long {return 0L;});

OP_GEN_ARITH_1_INP( mul_same_inp,
                    [] (const CiInt& a) -> CiInt {return a*a;},
                    [] (const long& a) -> long {return a*a;});

/**
 * Test two input arithmetic operators
 */
#define OP_GEN_ARITH_2_INP(TEST_NAME, OPER_CT, OPER_PT) \
TYPED_TEST(CiInt_OpGen, TEST_NAME) {                    \
  GEN_RAND_CI_L(x, rand()%32+1, rand()%2);              \
  GEN_RAND_CI_L(y, rand()%32, rand()%2);                \
  unsigned r_size = x_size;                             \
  bool r_is_signed = x_is_signed;                       \
  if (x_size == y_size) {                               \
    r_is_signed = x_is_signed and y_is_signed;          \
  } else if (x_size < y_size) {                         \
    r_size = y_size;                                    \
    r_is_signed = y_is_signed;                          \
  }                                                     \
  CiInt r = OPER_CT(x, y);                              \
  ASSERT_CI_PARAM(x, x_size, x_is_signed);              \
  ASSERT_EQ_CI_L(x, x_val);                             \
  ASSERT_CI_PARAM(y, y_size, y_is_signed);              \
  ASSERT_EQ_CI_L(y, y_val);                             \
  ASSERT_CI_PARAM(r, r_size, r_is_signed);              \
  ASSERT_EQ_CI_L(r, OPER_PT(x_val, y_val));             \
}

OP_GEN_ARITH_2_INP( add,
                    [] (const CiInt& a, const CiInt& b) -> CiInt {return a+b;},
                    [] (const long& a, const long& b) -> long {return a+b;});

OP_GEN_ARITH_2_INP( sub,
                    [] (const CiInt& a, const CiInt& b) -> CiInt {return a-b;},
                    [] (const long& a, const long& b) -> long {return a-b;});

OP_GEN_ARITH_2_INP( mul,
                    [] (const CiInt& a, const CiInt& b) -> CiInt {return a*b;},
                    [] (const long& a, const long& b) -> long {return a*b;});

/**
 * Test comparison operators applied on same input
 */
#define OP_GEN_COMP_1_INP(TEST_NAME, OPER_CT, OPER_PT)  \
TYPED_TEST(CiInt_OpGen, TEST_NAME) {                    \
  GEN_RAND_CI_L(x, rand()%32+1, rand()%2);              \
  CiBit r = OPER_CT(x);                                 \
  ASSERT_CI_PARAM(x, x_size, x_is_signed);              \
  ASSERT_EQ_CI_L(x, x_val);                             \
  ASSERT_EQ(r.get_val(), OPER_PT(x_val));               \
}

OP_GEN_COMP_1_INP(  equal_same_inp,
                    [] (const CiInt& a) -> CiBit {return a == a;},
                    [] (const long& a) -> bool {return true;});

OP_GEN_COMP_1_INP(  not_equal_same_inp,
                    [] (const CiInt& a) -> CiBit {return a != a;},
                    [] (const long& a) -> bool {return false;});

OP_GEN_COMP_1_INP(  lower_same_inp,
                    [] (const CiInt& a) -> CiBit {return a < a;},
                    [] (const long& a) -> bool {return false;});

OP_GEN_COMP_1_INP(  lower_equal_same_inp,
                    [] (const CiInt& a) -> CiBit {return a <= a;},
                    [] (const long& a) -> bool {return true;});

OP_GEN_COMP_1_INP(  greater_same_inp,
                    [] (const CiInt& a) -> CiBit {return a > a;},
                    [] (const long& a) -> bool {return false;});

OP_GEN_COMP_1_INP(  greater_equal_same_inp,
                    [] (const CiInt& a) -> CiBit {return a >= a;},
                    [] (const long& a) -> bool {return true;});

/**
 * Test comparison operators applied on two inputs
 */
#define OP_GEN_COMP_2_INP(TEST_NAME, OPER_CT, OPER_PT)  \
TYPED_TEST(CiInt_OpGen, TEST_NAME) {                    \
  GEN_RAND_CI_L(x, rand()%32+1, rand()%2);              \
  GEN_RAND_CI_L(y, rand()%32, rand()%2);                \
  CiBit r = OPER_CT(x, y);                              \
  ASSERT_CI_PARAM(x, x_size, x_is_signed);              \
  ASSERT_EQ_CI_L(x, x_val);                             \
  ASSERT_CI_PARAM(y, y_size, y_is_signed);              \
  ASSERT_EQ_CI_L(y, y_val);                             \
  x_val = mod(x_val, max(x_size, y_size));              \
  y_val = mod(y_val, max(x_size, y_size));              \
  ASSERT_EQ(r.get_val(), OPER_PT(x_val, y_val));        \
}

OP_GEN_COMP_2_INP(  equal,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a == b;},
                    [] (const long& a, const long& b) -> bool {return a == b;});

OP_GEN_COMP_2_INP(  not_equal,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a != b;},
                    [] (const long& a, const long& b) -> bool {return a != b;});

OP_GEN_COMP_2_INP(  lower,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a < b;},
                    [] (const long& a, const long& b) -> bool {return a < b;});

OP_GEN_COMP_2_INP(  lower_equal,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a <= b;},
                    [] (const long& a, const long& b) -> bool {return a <= b;});

OP_GEN_COMP_2_INP(  greater,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a > b;},
                    [] (const long& a, const long& b) -> bool {return a > b;});

OP_GEN_COMP_2_INP(  greater_equal,
                    [] (const CiInt& a, const CiInt& b) -> CiBit {return a >= b;},
                    [] (const long& a, const long& b) -> bool {return a >= b;});
