#include <gtest/gtest.h>

#include <ci_int.hxx>

using namespace std;
using namespace cingulata;

TEST(CiInt, constructor_from_ci_bit) {
  for (unsigned int i = 0; i < 100; i++) {
    CiBit b(rand() % 2);
    CiInt x(b);
    ASSERT_EQ(x.is_signed(), false);
    ASSERT_EQ(x.size(), 1);
    ASSERT_EQ(x[0].get_val(), b.get_val());
  }
}

TEST(CiInt, constructor_from_ci_bit_vector) {
  for (unsigned int i = 0; i < 100; i++) {
    int size = rand() % 128;
    vector<CiBit> v;
    for (unsigned int j = 0; j < size; j++) {
      v.push_back(CiBit(rand() % 2));
    }

    CiInt x(v);
    ASSERT_EQ(x.is_signed(), false);
    ASSERT_EQ(x.size(), v.size());
    ASSERT_EQ(x.size(), size);
    for (unsigned int j = 0; j < size; j++) {
      ASSERT_EQ(x[j].get_val(), v[j].get_val());
    }
  }
}

TEST(CiInt, encode_plain_val) {
  unsigned int bit_cnt[5] = {8, 16, 24, 32, 48};

  for (unsigned int i = 0; i < 100; i++) {
    int val = rand();
    for (unsigned int j = 0; j < 5; j++) {
      CiInt x(val, bit_cnt[j], false);
      ASSERT_EQ(x.is_signed(), false);
      for (unsigned int k = 0; k < x.size(); k++) {
        if ( k >= (8 * sizeof(val)) ) {
          ASSERT_EQ(x[k].get_val(), ((val >> (8 * sizeof(val) -1)) & 1));
        }
        else {
          ASSERT_EQ(x[k].get_val(), ((val >> k) & 1));
        }
      }

      CiInt y(val, bit_cnt[j], true);
      ASSERT_EQ(y.is_signed(), true);
      for (unsigned int k = 0; k < y.size(); k++) {
        if ( k >= (8 * sizeof(val)) ) {
          ASSERT_EQ(y[k].get_val(), ((val >> (8 * sizeof(val) -1)) & 1));
        }
        else {
          ASSERT_EQ(y[k].get_val(), ((val >> k) & 1));
        }
      }
    }

    CiInt z(val);
    ASSERT_EQ(z.is_signed(), true);
    for (unsigned int k = 0; k < z.size(); k++) {
      ASSERT_EQ(z[k].get_val(), ((val >> k) & 1));
    }
  }
}

TEST(CiInt, assign_from_ci_int) {
  CiInt x(rand());
  CiInt y = x;

  ASSERT_EQ(x.size(), y.size());
  ASSERT_EQ(x.is_signed(), y.is_signed());
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }
}

TEST(CiInt, assign_from_value) {
  int val = rand();
  CiInt x(val);
  CiInt y = val;

  ASSERT_EQ(x.size(), y.size());
  ASSERT_EQ(x.is_signed(), y.is_signed());
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }
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
  for (unsigned int i = 0; i < y.size(); i++) {
    if (i < x.size())
      ASSERT_EQ(x[i].get_val(), y[i].get_val());
    else
      ASSERT_EQ(y[i].get_val(), x.sign().get_val());
  }

  y = x.cast(x_size + delta, true);
  ASSERT_EQ(x.size(), x_size);
  ASSERT_EQ(y.size(), x.size() + delta);
  ASSERT_EQ(y.is_signed(), true);
  ASSERT_EQ(y.sign().get_val(), x.sign().get_val());
  for (unsigned int i = 0; i < y.size(); i++) {
    if (i < x.size())
      ASSERT_EQ(x[i].get_val(), y[i].get_val());
    else
      ASSERT_EQ(y[i].get_val(), x.sign().get_val());
  }
}

TEST(CiInt, is_signed) {
  CiInt x(rand(), 32, false);
  ASSERT_EQ(x.is_signed(), false);
  x = CiInt(rand(), 32, true);
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
  ASSERT_EQ(x.size(), x_size);
  ASSERT_EQ(y.size(), x.size() - delta);
  for (unsigned int i = 0; i < y.size(); i++) {
    ASSERT_EQ(x[i].get_val(), y[i].get_val());
  }

  int y_sign = y.sign().get_val();
  y = y.resize(y.size() + delta);
  ASSERT_EQ(y.size(), x.size());
  for (unsigned int i = 0; i < y.size(); i++) {
    if (i < (y.size() - delta))
      ASSERT_EQ(y[i].get_val(), x[i].get_val());
    else {
      ASSERT_EQ(y[i].get_val(), y_sign);
    }
  }
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
