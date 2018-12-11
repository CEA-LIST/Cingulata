#include <gtest/gtest.h>

#include <ci_int.hxx>

using namespace std;
using namespace cingulata;

TEST(CiInt, encode_plain_val) {
  unsigned int bit_cnt[5] = {8, 16, 24, 32, 48};

  for (unsigned int i = 0; i < 100; i++) {
    int val = rand();
    for (unsigned int j = 0; j < 5; j++) {
      CiInt x(val, bit_cnt[j], false);
      for (unsigned int k = 0; k < x.size(); k++) {
        if ( k >= (8 * sizeof(val)) ) {
          ASSERT_EQ(x[k].get_val(), ((val >> (8 * sizeof(val) -1)) & 1));
        }
        else {
          ASSERT_EQ(x[k].get_val(), ((val >> k) & 1));
        }
      }
      CiInt y(val, bit_cnt[j], true);
      for (unsigned int k = 0; k < y.size(); k++) {
        if ( k >= (8 * sizeof(val)) ) {
          ASSERT_EQ(y[k].get_val(), ((val >> (8 * sizeof(val) -1)) & 1));
        }
        else {
          ASSERT_EQ(y[k].get_val(), ((val >> k) & 1));
        }
      }
    }
  }
}

