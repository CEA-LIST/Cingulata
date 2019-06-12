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

#include <int_op_gen/interface.hxx>

using namespace std;
using namespace cingulata;

IIntOpGen::IIntOpGen()
    : m_multi_input_adder(
          bind(&IIntOpGen::add, this, placeholders::_1, placeholders::_2)),
      m_sort{bind(&IIntOpGen::lower, this, placeholders::_1, placeholders::_2),
             bind(&IIntOpGen::equal, this, placeholders::_1, placeholders::_2),
             bind(&IIntOpGen::add, this, placeholders::_1, placeholders::_2)} {}

CiBitVector IIntOpGen::sub(const CiBitVector &lhs,
                           const CiBitVector &rhs) const {
  return add(lhs, neg(rhs));
}

CiBitVector IIntOpGen::square(const CiBitVector &rhs) const {
  return mul(rhs, rhs);
}

CiBit IIntOpGen::not_equal(const CiBitVector &lhs,
                           const CiBitVector &rhs) const {
  return !equal(lhs, rhs);
}

CiBit IIntOpGen::greater(const CiBitVector &lhs, const CiBitVector &rhs) const {
  return lower(rhs, lhs);
}

CiBit IIntOpGen::lower_equal(const CiBitVector &lhs,
                             const CiBitVector &rhs) const {
  return !greater(lhs, rhs);
}

CiBit IIntOpGen::greater_equal(const CiBitVector &lhs,
                               const CiBitVector &rhs) const {
  return !lower(lhs, rhs);
}

CiBitVector IIntOpGen::mux(const CiBit &cond, const CiBitVector &a,
                           const CiBitVector &b) const {
  return mux(CiBitVector{1, cond}, {a, b});
}

CiBitVector IIntOpGen::mux(const CiBitVector &cond,
                           const vector<CiBitVector> &inps) const {
  return m_mux(cond, inps);
}

vector<CiBitVector> IIntOpGen::sort(const vector<CiBitVector> &v_cbv,
                                    const vector<CiBitVector> &i_cbv,
                                    const bool reverse) const {
  return m_sort(v_cbv, i_cbv, reverse);
}

CiBitVector IIntOpGen::sum(const vector<CiBitVector> &inps) const {
  return m_multi_input_adder(inps);
}
