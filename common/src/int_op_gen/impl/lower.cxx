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

#include <int_op_gen/impl/lower.hxx>

using namespace cingulata;
using namespace cingulata::int_ops;

CiBit LowerCompDepth::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int len = lhs.size();
  if (len == 1) return CiBit(lhs[0]).op_andny(rhs[0]);

  const int len2 = len>>1;
  CiBitVector lhs_l = lhs.slice(0,len2);
  CiBitVector lhs_h = lhs.slice(len2);
  CiBitVector rhs_l = rhs.slice(0,len2);
  CiBitVector rhs_h = rhs.slice(len2);

  return oper(lhs_h, rhs_h) ^ (equal(lhs_h, rhs_h) & oper(lhs_l, rhs_l));
}


CiBit LowerCompSize::oper(const CiBitVector& lhs, const CiBitVector& rhs) const {
  const int size = lhs.size();

  CiBit carry{0};
  for (int i = 0; i < size; ++i) {
    CiBit n1 = carry ^ lhs[i];
    CiBit n2 = carry ^ rhs[i];
    carry = (n1 & n2) ^ rhs[i];
  }
  return carry;
}
