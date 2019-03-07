#include <int_op_gen/impl/adder.hxx>
#include <int_op_gen/impl/multiplier.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

namespace
{
  std::vector<CiBitVector> reduce(const std::vector<CiBitVector>& tree) {
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
}

CiBitVector WallaceMultiplier::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  if (lhs.size() == 1) {
    CiBitVector res = lhs & rhs;
    return res;
  }
  else {
    std::vector<CiBitVector> tree;

    for (unsigned int i = 0; i < lhs.size(); ++i) {
        tree.push_back((rhs >> i) & lhs[i]);
    }

    while (tree.size() >= 3) {
      tree = reduce(tree);
    }

    CiBitVector res = adder(tree[0], tree[1]);

    return res;
  }
}
