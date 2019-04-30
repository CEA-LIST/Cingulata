#include <ci_context.hxx>
#include <ci_int.hxx>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cmath>

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
  CiInt VAR(VAR ## _val, VAR ## _size, VAR ## _is_signed);  \
  VAR.encrypt();


#define ASSERT_CI_PARAM(a, a_size, a_is_signed) \
{                                               \
  ASSERT_EQ((a).size(), a_size);                \
  ASSERT_EQ((a).is_signed(), a_is_signed);      \
}

template<typename T>
vector<int> decode(const T& val) {
  vector<int> res(val.size());
  for (int i = 0; i < val.size(); ++i)
    res[i] = val[i].get_val();
  return res;
}

template<typename T>
vector<int> decode_ext(const T& val, const unsigned size = -1) {
  vector<int> res = decode(val);

  if (size != (unsigned)-1) {
    if (val.is_signed()) {
      res.resize(size, val.sign().get_val());
    } else {
      res.resize(size, 0);
    }
  }

  return res;
}

template <typename T, typename = typename std::enable_if<
                          std::is_integral<T>::value, T>::type>
vector<int> to_binary(T val, const unsigned size = -1) {
  vector<int> res;
  for (int i = 0; i < 8*sizeof(T); ++i)
    res.push_back((val>>i)&1);

  if (size != (unsigned)-1) {
    if (std::is_signed<T>::value)
      res.resize(size, *(res.rbegin()));
    else
      res.resize(size, 0);
  }

  return res;
}

#define ASSERT_EQ_CI_L(v, val)                                                 \
  {                                                                            \
    auto _v = (v);                                                             \
    ASSERT_LE(_v.size(), 64);                                                  \
    auto _val = (val);                                                         \
    _v.decrypt();                                                              \
    vector<int> v_dec = decode(_v);                                            \
    vector<int> val_dec = to_binary(val, _v.size());                           \
    ASSERT_THAT(v_dec, ::testing::ElementsAreArray(val_dec));                  \
  }

#define ASSERT_EQ_CI_CI_S(a, b)                                                \
  {                                                                            \
    auto &aa = (a);                                                            \
    auto &bb = (b);                                                            \
    aa.decrypt();                                                              \
    bb.decrypt();                                                              \
    vector<int> a_dec = decode_ext(aa, bb.size());                             \
    vector<int> b_dec = decode_ext(bb, bb.size());                             \
    ASSERT_THAT(a_dec, ::testing::ElementsAreArray(b_dec));                    \
  }

#define ASSERT_EQ_CI_CI(a, b)                                                  \
  {                                                                            \
    if ((a).size() < (b).size()) {                                             \
      ASSERT_EQ_CI_CI_S((a), (b))                                              \
    } else {                                                                   \
      ASSERT_EQ_CI_CI_S((b), (a))                                              \
    }                                                                          \
  }

#define TEST_CONSTRUCTOR_CIBIT(_b_val, _n)                                     \
  {                                                                            \
    const auto b_val = (_b_val);                                               \
    const auto n = (_n);                                                       \
    CiInt x(CiBit(b_val).encrypt(), n);                                        \
    ASSERT_EQ(x.is_signed(), CiInt::default_is_signed) << b_val << " " << n;   \
    ASSERT_EQ(x.size(), n) << b_val << " " << n;                               \
    for (int i = 0; i < n; ++i)                                                \
      ASSERT_EQ(x[i].decrypt(), b_val) << b_val << " " << n;                   \
  }

TEST(CiInt, constructor_from_ci_bit_single) {
  TEST_CONSTRUCTOR_CIBIT(0, 0);
  TEST_CONSTRUCTOR_CIBIT(1, 0);
  TEST_CONSTRUCTOR_CIBIT(0, 1);
  TEST_CONSTRUCTOR_CIBIT(1, 1);
  TEST_CONSTRUCTOR_CIBIT(0, 2);
  TEST_CONSTRUCTOR_CIBIT(1, 2);
}

TEST(CiInt, constructor_from_ci_bit_rand) {
  TEST_CONSTRUCTOR_CIBIT(rand()%2, (rand()%64) + 3);
}

#define TEST_CONSTRUCTOR_CIBITVECTOR(_size)                                    \
  {                                                                            \
    const auto size = (_size);                                                 \
    vector<CiBit> v;                                                           \
    for (unsigned int j = 0; j < size; j++) {                                  \
      v.push_back(CiBit(rand() % 2));                                          \
    }                                                                          \
    CiInt x(v);                                                                \
    ASSERT_EQ(x.is_signed(), CiInt::default_is_signed);                        \
    ASSERT_THAT(decode(x), ::testing::ElementsAreArray(decode(v)));            \
  }

TEST(CiInt, constructor_from_ci_bit_vector_single) {
  TEST_CONSTRUCTOR_CIBITVECTOR(0);
  TEST_CONSTRUCTOR_CIBITVECTOR(1);
  TEST_CONSTRUCTOR_CIBITVECTOR(2);
}

TEST(CiInt, constructor_from_ci_bit_vector_rand) {
  TEST_CONSTRUCTOR_CIBITVECTOR((rand() % 128) + 2);
}

#define TEST_CONSTRUCTOR_PLAIN(_x_val)                                         \
  {                                                                            \
    auto x_val = (_x_val);                                                     \
    CiInt x(x_val);                                                            \
    unsigned x_size = (x_val < 0) ? (unsigned)ceil(log2(-x_val + 1)) + 1       \
                                  : (unsigned)ceil(log2(x_val + 1));           \
    ASSERT_EQ(x.size(), x_size);                                               \
    ASSERT_EQ(x.is_signed(), CiInt::default_is_signed);                        \
    ASSERT_THAT(to_binary(x_val, x_size),                                      \
                ::testing::ElementsAreArray(decode_ext(x, x_size)));           \
  }

#define TEST_CONSTRUCTOR_PLAIN_1(_x_val, _x_size)                              \
  {                                                                            \
    auto x_val = (_x_val);                                                     \
    auto x_size = (_x_size);                                                   \
    CiInt x(x_val, x_size);                                                    \
    ASSERT_EQ(x.size(), x_size);                                               \
    ASSERT_EQ(x.is_signed(), CiInt::default_is_signed);                        \
    ASSERT_THAT(to_binary(x_val, x_size),                                      \
                ::testing::ElementsAreArray(decode_ext(x, x_size)));           \
  }

#define TEST_CONSTRUCTOR_PLAIN_2(_x_val, _x_size, _x_is_signed)                \
  {                                                                            \
    auto x_val = (_x_val);                                                     \
    auto x_size = (_x_size);                                                   \
    auto x_is_signed = (_x_is_signed);                                         \
    CiInt x(x_val, x_size, x_is_signed);                                       \
    ASSERT_EQ(x.size(), x_size);                                               \
    ASSERT_EQ(x.is_signed(), x_is_signed);                                     \
    ASSERT_THAT(to_binary(x_val, x_size),                                      \
                ::testing::ElementsAreArray(decode_ext(x, x_size)));           \
  }

TEST(CiInt, constructor_plain_single) {
  TEST_CONSTRUCTOR_PLAIN((int32_t)15);
  TEST_CONSTRUCTOR_PLAIN((int32_t)-15);

  TEST_CONSTRUCTOR_PLAIN((uint32_t)15);
  TEST_CONSTRUCTOR_PLAIN((uint32_t)-15);

  TEST_CONSTRUCTOR_PLAIN((int32_t)16);
  TEST_CONSTRUCTOR_PLAIN((int32_t)-16);

  TEST_CONSTRUCTOR_PLAIN_1((int32_t)15, 4);
  TEST_CONSTRUCTOR_PLAIN_1((int32_t)-15, 4);

  TEST_CONSTRUCTOR_PLAIN_1((int32_t)15, 5);
  TEST_CONSTRUCTOR_PLAIN_1((int32_t)-15, 5);

  TEST_CONSTRUCTOR_PLAIN_1((uint32_t)15, 4);
  TEST_CONSTRUCTOR_PLAIN_1((uint32_t)-15, 4);

  TEST_CONSTRUCTOR_PLAIN_1((uint32_t)15, 5);
  TEST_CONSTRUCTOR_PLAIN_1((uint32_t)-15, 5);

  TEST_CONSTRUCTOR_PLAIN_2(15, 4, false);
  TEST_CONSTRUCTOR_PLAIN_2(-15, 4, false);

  TEST_CONSTRUCTOR_PLAIN_2(15, 4, true);
  TEST_CONSTRUCTOR_PLAIN_2(-15, 4, true);

  TEST_CONSTRUCTOR_PLAIN_2(15, 5, false);
  TEST_CONSTRUCTOR_PLAIN_2(-15, 5, false);

  TEST_CONSTRUCTOR_PLAIN_2(15, 5, true);
  TEST_CONSTRUCTOR_PLAIN_2(-15, 5, true);
}

TEST(CiInt, assign_from_ci_int) {
  GEN_RAND_CI_L(x, rand() % 64, rand() % 2);
  CiInt y = x;

  ASSERT_CI_PARAM(x, x_size, x_is_signed);
  ASSERT_EQ_CI_L(x, x_val);

  ASSERT_CI_PARAM(y, x_size, x_is_signed);
  ASSERT_EQ_CI_L(y, x_val);
}

TEST(CiInt, assign_from_value_single) {
  {
    const int val = 0;
    const unsigned val_size = 0;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const int val = 1;
    const unsigned val_size = 1;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const int val = -1;
    const unsigned val_size = 2;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const unsigned val = 0;
    const unsigned val_size = 0;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const unsigned val = 1;
    const unsigned val_size = 1;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const unsigned val = -1;
    const unsigned val_size = sizeof(unsigned)*8;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }}

TEST(CiInt, assign_from_value) {
  {
    const int val = rrand(0, 1<<5);
    const unsigned val_size = (unsigned)ceil(log2(val+1));
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const int val = rrand(-(1<<5), -1);
    const unsigned val_size = (unsigned)ceil(log2(-val+1))+1;
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const unsigned val = rrand(0, 1<<5);
    const unsigned val_size = (unsigned)ceil(log2(val+1));
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
    ASSERT_EQ_CI_L(y, val);
  }
  {
    const unsigned val = rrand(-(1<<5), -1);
    const unsigned val_size = 8*sizeof(unsigned);
    CiInt y = val;
    ASSERT_CI_PARAM(y, val_size, CiInt::default_is_signed);
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

TEST(CiInt, sign_single) {
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

  const int min_size = min(y.size(), z.size());
  for (unsigned int i = 0; i < min_size; i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() & z[i].get_val());
  }
  for (unsigned int i = min_size; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() & z.sign().get_val());
  }
}

TEST(CiInt, or_operand) {
  CiInt x(rand());
  CiInt y = x;
  y |= y;
  ASSERT_EQ_CI_CI(x, y);

  CiInt z(rand());
  y |= z;

  const int min_size = min(y.size(), z.size());
  for (unsigned int i = 0; i < min_size; i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() | z[i].get_val());
  }
  for (unsigned int i = min_size; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() | z.sign().get_val());
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

  const int min_size = min(y.size(), z.size());
  for (unsigned int i = 0; i < min_size; i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() ^ z[i].get_val());
  }
  for (unsigned int i = min_size; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() ^ z.sign().get_val());
  }
}

TEST(CiInt, right_shift) {
  const CiInt x(rand());
  const int n = x.size();
  int delta = rand() % (n);
  const auto x_dec = decode(x);

  /* right shift with positive value*/
  {
    vector<int> xp_dec(n, x.sign().get_val());
    for (int i = 0; i < n - delta; ++i)
      xp_dec[i] = x_dec[i + delta];

    CiInt y(x);
    y >>= delta;

    ASSERT_THAT(decode(y), ::testing::ElementsAreArray(xp_dec));

    CiInt z = x >> delta;
    ASSERT_THAT(decode(z), ::testing::ElementsAreArray(xp_dec));
  }

  /* right shift with a negative value */
  {
    CiInt y1(x);
    y1 >>= (-delta);

    CiInt y2(x);
    y2 <<= (delta);

    ASSERT_THAT(decode(y1), ::testing::ElementsAreArray(decode(y2)));

    CiInt z1 = x >> (-delta);
    CiInt z2 = x << (delta);

    ASSERT_THAT(decode(z1), ::testing::ElementsAreArray(decode(z2)));
  }
}

TEST(CiInt, left_shift) {
  const CiInt x(rand());
  const int n = x.size();
  int delta = rand() % (n);
  const auto x_dec = decode(x);

  /* left shift with positive value*/
  {
    vector<int> xp_dec(n, 0);
    for (int i = 0; i < n - delta; ++i)
      xp_dec[i + delta] = x_dec[i];

    CiInt y(x);
    y <<= delta;

    ASSERT_THAT(decode(y), ::testing::ElementsAreArray(xp_dec));

    CiInt z = x << delta;
    ASSERT_THAT(decode(z), ::testing::ElementsAreArray(xp_dec));
  }

  /* left shift with a negative value */
  {
    CiInt y1(x);
    y1 <<= (-delta);

    CiInt y2(x);
    y2 >>= (delta);

    ASSERT_THAT(decode(y1), ::testing::ElementsAreArray(decode(y2)));

    CiInt z1 = x << (-delta);
    CiInt z2 = x >> (delta);

    ASSERT_THAT(decode(z1), ::testing::ElementsAreArray(decode(z2)));
  }
}

TEST(CiInt, rol) {
  const CiInt x(rand());
  const int n = x.size();
  int delta = rand() % (n);
  const auto x_dec = decode(x);

  /* left rotate with positive value*/
  {
    vector<int> xp_dec(n, 0);
    for (int i = 0; i < n; ++i)
      xp_dec[(i + delta) % n] = x_dec[i];

    CiInt y(x);
    y.rol(delta);

    ASSERT_THAT(decode(y), ::testing::ElementsAreArray(xp_dec));
  }

  /* left rotate with negative value*/
  {
    CiInt y1(x);
    y1.rol(-delta);

    CiInt y2(x);
    y2.ror(delta);

    ASSERT_THAT(decode(y1), ::testing::ElementsAreArray(decode(y2)));
  }
}

TEST(CiInt, ror) {
  const CiInt x(rand());
  const int n = x.size();
  int delta = rand() % (n);
  const auto x_dec = decode(x);

  /* left rotate with positive value*/
  {
    vector<int> xp_dec(n, 0);
    for (int i = 0; i < n; ++i)
      xp_dec[i] = x_dec[(i + delta) % n];

    CiInt y(x);
    y.ror(delta);

    ASSERT_THAT(decode(y), ::testing::ElementsAreArray(xp_dec));
  }

  /* left rotate with negative value*/
  {
    CiInt y1(x);
    y1.ror(-delta);

    CiInt y2(x);
    y2.rol(delta);

    ASSERT_THAT(decode(y1), ::testing::ElementsAreArray(decode(y2)));
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
  ASSERT_EQ(r.decrypt(), OPER_PT(x_val));               \
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
  ASSERT_EQ(r.decrypt(), OPER_PT(x_val, y_val));        \
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
