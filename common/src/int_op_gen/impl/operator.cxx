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

#include <cassert>

#include <int_op_gen/impl/operator.hxx>

using namespace std;
using namespace cingulata;
using namespace cingulata::int_ops;

CiBitVector UnaryOper::operator()(const CiBitVector& rhs) const {
  assert(rhs.size() > 0);

  return oper(rhs);
}

CiBitVector BinaryOper::operator()(const CiBitVector& lhs, const CiBitVector& rhs) const {
  assert(lhs.size() == rhs.size());
  assert(lhs.size() > 0);

  return oper(lhs, rhs);
}

CiBitVector NaryOper::operator()(const vector<CiBitVector>& inps) const {
  if (inps.size() == 0) {
    return CiBitVector();
  } else if (inps.size() == 1) {
    return inps[0];
  } else {
    vector<CiBitVector> m_inps;
    for (const CiBitVector& inp: inps)
      if (inp.size() > 0) m_inps.push_back(inp);
    if (m_inps.empty())
      return CiBitVector();
    else
      return oper(m_inps);
  }
}

CiBit CompOper::operator()(const CiBitVector& lhs, const CiBitVector& rhs) const {
  assert(lhs.size() == rhs.size());
  assert(lhs.size() > 0);

  return oper(lhs, rhs);
}

CiBitVector MuxOper::operator()(const CiBitVector& cond, const vector<CiBitVector>& p_inps) const {
  /* resize inputs to same bit-size */
  vector<CiBitVector> inps = p_inps;

  int max_size = 0;
  for (const auto& inp: inps)
    if (max_size < (int)inp.size())
      max_size = inp.size();

  if (max_size == 0)
    return CiBitVector();

  for (auto& inp: inps)
    inp.resize(max_size);

  assert((1U<<cond.size()) == inps.size());

  return oper(cond, inps);
}

vector<CiBitVector> SortOper::operator()(const vector<CiBitVector> &v_cbv,
                                         const vector<CiBitVector> &i_cbv,
                                         const bool reverse) const {
  if (v_cbv.size() == 0)
    return vector<CiBitVector>();

  assert(v_cbv.size() == i_cbv.size());

  if (v_cbv.size() == 1)
    return i_cbv;

  auto s = v_cbv[0].size();
  for (const auto &cbv : v_cbv)
    assert(cbv.size() == s);
  for (const auto &cbv : i_cbv)
    assert(cbv.size() == s);
  return oper(v_cbv, i_cbv, reverse);
}

vector<CiBitVector> SortOper::operator()(const vector<CiBitVector> &v_cbv,
                                         const bool reverse) const {
  return (*this)(v_cbv, v_cbv, reverse);
}
