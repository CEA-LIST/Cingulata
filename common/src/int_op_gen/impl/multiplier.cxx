#include <int_op_gen/impl/adder.hxx>
#include <int_op_gen/impl/multiplier.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector WallaceMultiplier::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const unsigned out_size = lhs.size();
  std::vector<CiBitVector> tree;

  for (unsigned int i = 0; i < out_size; ++i) {
    if (lhs[i].get_val() == 0) {
      tree.push_back(CiBitVector(out_size, CiBit::zero));
    }
    else {
      tree.push_back(CiBitVector(rhs >> i));
    }
  }

  while (tree.size() >= 3) {
    tree = reduce(tree);
  }

  BinaryOper *adder = new SklanskyAdder();
  CiBitVector res = (*adder)(tree[0], tree[1]);

  return res;
}

CiBitVector WallaceMultiplier::multiply(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const unsigned out_size = lhs.size() + rhs.size() - 1;
  CiBitVector short_v = (lhs.size() > rhs.size()) ? rhs : lhs;
  CiBitVector long_v  = (lhs.size() > rhs.size()) ? lhs : rhs;
  std::vector<CiBitVector> tree;

  for (unsigned int i = 0; i < short_v.size(); ++i) {
    if (short_v[i].get_val() == 0) {
      tree.push_back(CiBitVector(out_size, CiBit::zero));
    }
    else {
      if (i == 0) {
        auto tmp = long_v;
        tmp.insert(tmp.end(), (out_size - long_v.size()), CiBit::zero);
        tree.push_back(tmp);
      }
      else {
        CiBitVector tmp(i, CiBit::zero);
        tmp.insert(tmp.end(), long_v.begin(), long_v.end());
        tmp.insert(tmp.end(), (out_size - i - long_v.size()), CiBit::zero);
        tree.push_back(tmp);
      }
    }
  }

  while (tree.size() >= 3) {
    tree = reduce(tree);
  }

  BinaryOper *adder = new SklanskyAdder();
  CiBitVector res = (*adder)(tree[0], tree[1]);

  return res;
}

std::vector<CiBitVector> WallaceMultiplier::reduce(const std::vector<CiBitVector>& tree) const {
  const unsigned iter = (unsigned int)(tree.size() / 3);
  const unsigned mod = tree.size() % 3;
  std::vector<CiBitVector> res;

  for (unsigned int i = 0; i < iter; ++i) {
    auto sum = tree[3*i];
    sum ^= (tree[3*i+1] ^ tree[3*i+2]);
    res.push_back(sum);

    CiBitVector carry(tree[3*i].size(), CiBit::zero);
    for (unsigned int j = 0; j < (tree[3*i].size() - 1); ++j) {
      carry[j+1] = ((tree[3*i][j] ^ tree[3*i+2][j]) & (tree[3*i+1][j] ^ tree[3*i+2][j])) ^ tree[3*i+2][j];
    }
    res.push_back(carry);
  }

  if (mod != 0) {
    res.insert(res.end(), tree.end()-mod, tree.end());
  }

  const unsigned res_size = 2 * iter + mod;
  assert(res.size() == res_size);

  return res;
}
