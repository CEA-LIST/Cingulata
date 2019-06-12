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

#include <int_op_gen/impl/equal.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBit EqualDepth::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBitVector tmp(size, 0);
  for (int i = 0; i < size; ++i)
    tmp[i] = lhs[i] == rhs[i];

  /* log depth tree */
  for (int k = 1; k < size; k *= 2) {
    for (int i = 0; i < size - k; i += 2*k) {
      tmp[i] &= tmp[i+k];
    }
  }

  return tmp[0];
}

CiBit EqualSize::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit res{1};
  for (int i = 0; i < size; ++i) {
    res &= lhs[i] == rhs[i];
  }

  return res;
}
