#include <int_op_gen/impl/adder.hxx>
#include <math.h>
#include <iostream>

using namespace cingulata;
using namespace cingulata::int_ops;


CiBitVector RippleCarryAdder::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  int size = lhs.size();
  CiBitVector res(size);

  CiBit carry{0};
  for (int i = 0; i < size; ++i) {
    CiBit n1 = carry ^ lhs[i];
    CiBit n2 = carry ^ rhs[i];
    res[i] = n1 ^ rhs[i];
    if (i < size-1)
      carry = (n1 & n2) ^ carry;
  }

  return res;
}

namespace
{
  void pre_computation( std::vector<CiBitVector>& P,  std::vector<CiBitVector>& G, const CiBitVector& lhs, const CiBitVector& rhs) {
    const int size = lhs.size();
    CiBit test();
    for (int i = 0; i < size; ++i) {
      P[i][i] = lhs[i] ^ rhs[i];
      G[i][i] = lhs[i] & rhs[i];
    }
  }

  void evaluate_G( std::vector<CiBitVector>& P,  std::vector<CiBitVector>& G, int row_index, int col_index, int step) {
    int k = col_index + (int) pow(2,step-1);
    G[row_index][col_index] = G[row_index][k] ^ (P[row_index][k] & G[k-1][col_index]);
  }


  void evaluate_P( std::vector<CiBitVector>& P,  std::vector<CiBitVector>& G, int row_index, int col_index, int step) {
    int k = col_index + (int) pow(2,step-1);
    P[row_index][col_index] = P[row_index][k] & P[k-1][col_index];
  }

  CiBitVector post_computation( std::vector<CiBitVector>& P, std::vector<CiBitVector>& G, int size) {
    CiBitVector res(size);
    res[0] = P[0][0];
    for ( int i = 1; i < size;  ++i)
      res[i] = P[i][i] ^ G[i-1][0];
    return res;
  }
}


CiBitVector SklanskyAdder::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  CiBitVector res;

  int size = lhs.size();
  std::vector<CiBitVector> P(size, CiBitVector(size));
  std::vector<CiBitVector> G(size, CiBitVector(size));
  int num_steps = (int) floor(log2((double) size )) + 1;

  /* compute initial G and P*/
  pre_computation(P, G, lhs, rhs);

  /* do for each level */
  for (int step = 1; step <= num_steps; ++step ) {
    int row = 0;
    int col = 0;
    /* shift row */
    row +=  (int) pow(2,step-1);
    /*   do while the size of enter is not reach*/
    while (row < size) {
      /* define column value */
      col = (int) floor(row/pow(2,step)) * (int) pow(2,step);
      for (int i = 0; i <(int) pow(2,step-1); ++i) {
        evaluate_G(P, G, row, col, step);
        if (col != 0)
          evaluate_P(P, G, row, col, step);
        /* increment row value */
        row += 1;
        /* case size is not multiple of 2 */
        if (row == size)
          break;
      }
      row +=  (int) pow(2,step-1);
    }

  }
  /* compute results*/
  res = post_computation(P, G, size);

  return res;
}


