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

#ifndef SORT_OPER
#define SORT_OPER

#include <int_op_gen/impl/operator.hxx>

#include <functional>
#include <cassert>

namespace cingulata
{
namespace int_ops
{

/**
 * @brief      Sort an array of integers
 * @details    The sort function sort a vector of CiInt and return the values.
 *             The multiplicative depth of this implementation is minimal.
 * @note       Implementation described by Cetin & al in *Low Depth Circuits for
 *             Efficient Homomorphic Sorting*.
 */
class SortDepth : public SortOper {
public:
  SortDepth(const std::function<CompOper::signature> &cmp,
            const std::function<CompOper::signature> &equ,
            const std::function<BinaryOper::signature> &adder)
      : cmp(cmp), equ(equ), adder(adder) {}

private:
  std::vector<CiBitVector> oper(const std::vector<CiBitVector> &v_cbv,
                                const std::vector<CiBitVector> &i_cbv,
                                const bool reverse) const;

  CiBitVector hammingWeight(const CiBitVector x, unsigned int l,
                            unsigned int size_ham) const;

  std::function<CompOper::signature> cmp;

  std::function<CompOper::signature> equ;

  std::function<BinaryOper::signature> adder;
};
}
}
#endif
