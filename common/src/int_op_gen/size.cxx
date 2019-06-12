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

#include <int_op_gen/size.hxx>

using namespace cingulata;

IntOpGenSize::IntOpGenSize() : m_neg{m_add}, m_mul{m_add} {}

CiBitVector IntOpGenSize::add(const CiBitVector &lhs,
                              const CiBitVector &rhs) const {
  return m_add(lhs, rhs);
}

CiBitVector IntOpGenSize::neg(const CiBitVector &lhs) const {
  return m_neg(lhs);
}

CiBitVector IntOpGenSize::mul(const CiBitVector &lhs,
                              const CiBitVector &rhs) const {
  return m_mul(lhs, rhs);
}

CiBit IntOpGenSize::equal(const CiBitVector &lhs,
                          const CiBitVector &rhs) const {
  return m_equal(lhs, rhs);
}

CiBit IntOpGenSize::lower(const CiBitVector &lhs,
                          const CiBitVector &rhs) const {
  return m_lower(lhs, rhs);
}
