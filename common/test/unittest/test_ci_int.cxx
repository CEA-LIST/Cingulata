#include <gtest/gtest.h>

#include <ci_int.hxx>

using namespace std;
using namespace cingulata;


template<typename T>
T get_val(const CiInt& a) {
  T res = 0;

  for (unsigned i = 0; i < a.size(); ++i) {
    res += (a[i].get_val() & 1) << i;
  }

  for (int i = a.size(); i < sizeof(T)*8; ++i) {
    res += (a.sign().get_val() & 1) << i;
  }

  return res;
}


TEST(CiInt, constructor_from_ci_bit) {
  CiBit b(rand() % 2);
  CiInt x(b);
  ASSERT_EQ(x.is_signed(), false);
  ASSERT_EQ(x.size(), 1);
  ASSERT_EQ(get_val<CiBit::bit_plain_t>(x), b.get_val());
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
  unsigned int bit_cnt[5] = {8, 16, 24, 32, 48};

  int val = rand();
  for (unsigned int j = 0; j < 5; j++) {
    CiInt x(val, bit_cnt[j], false);
    ASSERT_EQ(x.is_signed(), false);
    ASSERT_EQ(get_val<long>(x), val % (1L<<bit_cnt[j])) << " bit count = " << bit_cnt[j];

    CiInt y(val, bit_cnt[j], true);
    ASSERT_EQ(y.is_signed(), true);
    ASSERT_EQ(get_val<long>(x), val % (1L<<bit_cnt[j])) << " bit count = " << bit_cnt[j];
  }

  CiInt z(val);
  ASSERT_EQ(z.is_signed(), true);
  ASSERT_EQ(get_val<long>(z), val % (1L<<sizeof(int)*8));
}

TEST(CiInt, assign_from_ci_int) {
  CiInt x(rand());
  CiInt y = x;

  ASSERT_EQ(x.size(), y.size());
  ASSERT_EQ(x.is_signed(), y.is_signed());
  ASSERT_EQ(get_val<long>(x), get_val<long>(y));
}

TEST(CiInt, assign_from_value) {
  int val = rand();
  CiInt x(val);
  CiInt y = val;

  ASSERT_EQ(x.size(), y.size());
  ASSERT_EQ(x.is_signed(), y.is_signed());
  ASSERT_EQ(get_val<long>(x), val);
}

TEST(CiInt, cast) {
  CiInt x(rand());
  int x_size = x.size();
  int delta = rand() % (x_size);

  CiInt y = x.cast(x_size + delta, false);
  ASSERT_EQ(x.size(), x_size);
  ASSERT_EQ(y.size(), x.size() + delta);
  ASSERT_EQ(y.is_signed(), false);
  ASSERT_EQ(y.sign().get_val(), 0);
  ASSERT_EQ(get_val<long>(x), get_val<long>(y));

  y = x.cast(x_size + delta, true);
  ASSERT_EQ(x.size(), x_size);
  ASSERT_EQ(y.size(), x.size() + delta);
  ASSERT_EQ(y.is_signed(), true);
  ASSERT_EQ(y.sign().get_val(), x.sign().get_val());
  ASSERT_EQ(get_val<long>(x), get_val<long>(y));
}

TEST(CiInt, is_signed) {
  CiInt x(rand(), rand() % 128, false);
  ASSERT_EQ(x.is_signed(), false);
  x = CiInt(rand(), rand() % 128, true);
  ASSERT_EQ(x.is_signed(), true);
}

TEST(CiInt, change_sign) {
  CiInt x(rand());
  ASSERT_EQ(x.change_sign(true).is_signed(), true);
  ASSERT_EQ(x.change_sign(false).is_signed(), false);
  ASSERT_EQ(x.to_signed().is_signed(), true);
  ASSERT_EQ(x.to_unsigned().is_signed(), false);
}

TEST(CiInt, resize) {
  CiInt x(rand());
  int x_size = x.size();
  int delta = rand() % (x_size);

  CiInt y = x.resize(x_size - delta);
  int y_sign = y.sign().get_val();
  ASSERT_EQ(x.size(), x_size);
  ASSERT_EQ(y.size(), x.size() - delta);
  ASSERT_EQ(get_val<long>(x) % (1L << y.size()), get_val<long>(y) % (1L << y.size()) + y_sign);

  int y_size = y.size();
  y = y.resize(y.size() + delta);
  ASSERT_EQ(y.size(), x.size());
  ASSERT_EQ(get_val<long>(x) % (1L << y_size), get_val<long>(y) % (1L << y_size) + y_sign);
  ASSERT_EQ((get_val<long>(y) >> y_size) % (1L << delta), (y_sign << (delta))-y_sign);
}

TEST(CiInt, lsb) {
  CiInt x(rand());
  ASSERT_EQ(x.lsb().get_val(), x[0].get_val());
}

TEST(CiInt, msb) {
  CiInt x(rand());
  ASSERT_EQ(x.msb().get_val(), x[x.size()-1].get_val());
}

TEST(CiInt, sign) {
  CiInt x(255, 8, false);
  ASSERT_EQ(x.sign().get_val(), 0);

  x = CiInt(255, 8, true);
  ASSERT_EQ(x.sign().get_val(), 1);
}

TEST(CiInt, bits) {
  CiInt x(rand());
  auto y = x.bits();
  ASSERT_EQ(x.size(), y.size());
  for (unsigned int i = 0; i < x.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }
}

TEST(CiInt, and_operand) {
  CiInt x(rand());
  CiInt y = x;
  y &= y;
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }

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
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }

  CiInt z(rand());
  y |= z;
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(y[i].get_val(), x[i].get_val() | z[i].get_val());
  }
}

TEST(CiInt, xor_operand) {
  CiInt x(rand());
  x ^= x;
  for (unsigned int i = 0; i < x.size(); i++) {
    ASSERT_EQ(x[i].get_val(), 0);
  }

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
