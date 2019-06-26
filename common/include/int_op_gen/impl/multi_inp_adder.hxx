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

#ifndef MULTI_INP_ADDER_OPER
#define MULTI_INP_ADDER_OPER

#include <int_op_gen/impl/operator.hxx>

namespace cingulata
{
namespace int_ops
{
/**
 * @brief      Adds together several bit vectors. Inputs bit-sizes can be
 *             different. The smallest possible bit-sized output bit vector is
 *             returned (e.g. for @c n inputs of same size @c m the output size
 *             is @c m+ceil(log2(n))).
 */
class MultiInputAdder : public NaryOper {
public:
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  p_bin_adder  Binary adder to use for adding 2 values
   */
  MultiInputAdder(const std::function<BinaryOper::signature>& p_bin_adder);

private:

  /**
   * @brief      Implementation
   *
   * @param[in]  inps  Array of bit vectors to add
   *
   * @return     Sum of input bit vectors
   */
  CiBitVector oper(const std::vector<CiBitVector> &inps) const override;

  CiBitVector add(const std::vector<CiBitVector> &inps, const unsigned max_bit_size = -1) const;

  std::function<BinaryOper::signature> bin_adder;
};

} // namespace int_ops
} // namespace cingulata

#endif
