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

#ifndef ADDER_OPER
#define ADDER_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata {
namespace int_ops {

/**
 * @brief      Riple-carry adder operator (small size)
 * @details    The multiplicative depth of generated circuit is @c
 *             lhs.size(). It has @c lhs.size() AND gates and @c
 *             4*lhs.size()-1 XOR gates.
 * @note       Circuit described in Kolesnikov V, Sadeghi AR, Schneider T.
 *             *Improved garbled circuit building blocks and applications to
 *             auctions and computing minima*
 */
class RippleCarryAdder : public AdderOper {
  /**
   * @brief      Implementations
   * @details    The size of result is the size of @c lhs.
   *
   * @param[in]  lhs   The left hand side
   * @param[in]  rhs   The right hand side
   *
   * @return     sum bit-vector
   */
  CiBitVector oper(const CiBitVector &lhs, const CiBitVector &rhs,
                   const CiBit &carry_in) const override;
};
/**
 * @brief      Sklansky adder operator (minimum multiplicative depth)
 * @details    The multiplicative depth of generated circuit is log2(
 * lhs.size())+1 @c It has @c lhs.size()*log(lhs.size()) AND gates and @
 *             4*lhs.size()-1 XOR gates.
 * @note       Circuit described in David Harris.
 *             *A taxonomy of Parallel Prefix Networks*
 */
class SklanskyAdder : public AdderOper {
  /**
   * @brief      Implementations
   * @details    The size of result is the size of @c lhs.
   *
   * @param[in]  lhs   The left hand side
   * @param[in]  rhs   The right hand side
   * @remark     currently, the implementation does not take into account the @c
   * carry_in bit
   *
   * @return     sum bit-vector
   */
  CiBitVector oper(const CiBitVector &lhs, const CiBitVector &rhs,
                   const CiBit &carry_in) const override;
};

} // namespace int_ops
} // namespace cingulata

#endif
