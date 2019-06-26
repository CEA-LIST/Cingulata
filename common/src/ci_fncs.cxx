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

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_context.hxx>
#include <ci_fncs.hxx>
#include <ci_int.hxx>

using namespace std;

namespace cingulata {

namespace
{
  template<typename out_t, typename inp_t>
  out_t cast(const inp_t& val);

  template<> CiBitVector cast<CiBitVector, CiBit>(const CiBit& val) {
    return CiBitVector(1, val);
  }

  template<> CiBitVector cast<CiBitVector, CiInt>(const CiInt& val) {
    return val.cast();
  }

  template<> CiInt cast<CiInt, CiBitVector>(const CiBitVector& val) {
    return val;
  }

  template<typename out_t, typename inp_t>
  vector<out_t> vcast(const vector<inp_t>& vals) {
    vector<out_t> out_vals;
    for (const inp_t& val: vals)
      out_vals.push_back(cast<out_t, inp_t>(val));
    return out_vals;
  }
}


CiInt select(const CiBit &cond, const CiInt &a, const CiInt &b) {
  return CiContext::get_int_op_gen()->mux(cond, b.cast(), a.cast());
}

CiInt sum(const vector<CiInt> &vals) {
  const auto m_vals = vcast<CiBitVector>(vals);
  return CiContext::get_int_op_gen()->sum(m_vals);
}

CiInt sum(const vector<CiBit> &vals) {
  const auto m_vals = vcast<CiBitVector>(vals);
  return CiContext::get_int_op_gen()->sum(m_vals);
}

vector<CiInt> sort(const vector<CiInt> &vals, const bool reverse) {
  const auto m_vals = vcast<CiBitVector>(vals);
  const vector<CiBitVector> res = CiContext::get_int_op_gen()->sort(m_vals, m_vals, reverse);
  return vcast<CiInt>(res);
}

vector<CiInt> sort(const vector<CiInt> &vals, const vector<CiInt> &elems,
                   const bool reverse) {
  const auto m_vals = vcast<CiBitVector>(vals);
  const auto m_elems = vcast<CiBitVector>(elems);
  const vector<CiBitVector> res = CiContext::get_int_op_gen()->sort(m_vals, m_elems, reverse);
  return vcast<CiInt>(res);
}

vector<CiInt> argsort(const vector<CiInt> &vals,
                        const bool reverse) {
  const auto m_vals = vcast<CiBitVector>(vals);

  vector<CiBitVector> indices;
  for (unsigned i = 0; i < vals.size(); ++i) {
    indices.push_back(encode_plain_int(i));
  }

  const vector<CiBitVector> res = CiContext::get_int_op_gen()->sort(m_vals, indices, reverse);
  return vcast<CiInt>(res);
}



}
