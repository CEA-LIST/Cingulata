#include <int_op_gen/impl/sort.hxx>
#include <utils.hxx>

#include <cmath>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector SortDepth::hammingWeight(const CiBitVector x, unsigned int l,
                                     unsigned int size_ham) const {
  unsigned int m = floor(l / 2);
  unsigned int n = l - m;
  if (l == 1) {
    CiBitVector r(size_ham);
    r[0] = x[0];
    return r;
  }
  CiBitVector left(m);
  CiBitVector right(n);
  for (unsigned int i = 0; i < m; i++) {
    left[i] = x[i];
  }

  for (unsigned int i = m; i < m + n; i++) {
    right[i - m] = x[i];
  }
  return (adder(hammingWeight(left, m, size_ham),
                hammingWeight(right, n, size_ham)));
}

vector<CiBitVector> SortDepth::oper(const vector<CiBitVector> &v_cbv,
                                    const vector<CiBitVector> &i_cbv,
                                    const bool reverse) const {
  unsigned int size = v_cbv[0].size();
  unsigned int N = v_cbv.size();
  vector<CiBitVector> res(N, CiBitVector(size));
  vector<CiBitVector> m(N, CiBitVector(N));
  for (unsigned int i = 0; i < N; i++) {
    m[i][i] = CiBit(false);
    for (unsigned int j = i + 1; j < N; j++) {
      m[i][j] = cmp(v_cbv[i], v_cbv[j]) ^ CiBit(reverse);
      m[j][i] = m[i][j] ^ CiBit(true);
    }
  }

  for (unsigned int i = 0; i < N; i++) {
    m[i][i] = CiBit(false);
    for (unsigned int j = i + 1; j < N; j++) {
      CiBit aux = m[i][j];
      m[i][j] = m[j][i];
      m[j][i] = aux;
    }
  }

  unsigned int size_ham = ceil(log2(N)) + 1;
  vector<CiBitVector> hamming_weights(N, CiBitVector(size_ham));
  for (unsigned int i = 0; i < N; i++) {
    hamming_weights[i] = SortDepth::hammingWeight(m[i], N, size_ham);
  }

  for (unsigned int i = 0; i < N; i++) {
    for (unsigned int j = 0; j < N; j++) {
      CiBit z;
      CiBitVector i_bit_vec(encode_plain_int(i, size_ham));
      z = (equ(i_bit_vec, hamming_weights[j]));
      res[i] ^= CiBitVector(size, z) & i_cbv[j];
    }
  }

  return res;
}
