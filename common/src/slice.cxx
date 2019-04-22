#include <slice.hxx>

#include <cassert>

using namespace std;
using namespace cingulata;

/* Base slice */

SliceBase::SliceBase(const int p_list_size, const optional<int>& p_start,
        const optional<int>& p_end, const optional<int>& p_stride) {
  m_stride = p_stride.value_or(1);
  assert(m_stride != 0);

  m_start = p_start.value_or(m_stride > 0 ? 0 : p_list_size - 1);
  m_end = p_end.value_or(m_stride > 0 ? p_list_size : -1);
  m_size = (m_end - m_start) / m_stride;
}

unsigned SliceBase::size() const {
  return m_size;
}

unsigned SliceBase::abs_idx(const unsigned rel_idx) const {
  return m_start + rel_idx * m_stride;
}


/* Slice */

template <typename list_t>
Slice<list_t>::Slice(list_t& p_obj, const optional<int> &p_start,
                     const optional<int> &p_end, const optional<int> &p_stride)
    : SliceBase(p_obj.size(), p_start, p_end, p_stride), m_obj(p_obj) {
}

namespace {
template <typename T1, typename T2>
T1 &assign(T1 &to, const T2 &from) {
  int n = to.size();
  if (n > (int)from.size())
    n = from.size();
  for (int i = 0; i < n; ++i) {
    to[i] = from[i];
  }
  return to;
}
} // namespace

template <typename list_t>
Slice<list_t>& Slice<list_t>::operator=(const Slice& other) {
  return assign(*this, other);
}

template <typename list_t>
Slice<list_t>& Slice<list_t>::operator=(const CSlice<list_t>& other) {
  return assign(*this, other);
}

template <typename list_t>
typename list_t::elem_t& Slice<list_t>::operator[](const unsigned idx) {
  assert(idx < size());
  return m_obj.at(abs_idx(idx));
}

template <typename list_t>
const typename list_t::elem_t& Slice<list_t>::operator[](const unsigned idx) const {
  assert(idx < size());
  return m_obj.at(abs_idx(idx));
}

/* Constant slice */

template <typename list_t>
CSlice<list_t>::CSlice(const list_t& p_obj, const optional<int> &p_start,
                     const optional<int> &p_end, const optional<int> &p_stride)
    : SliceBase(p_obj.size(), p_start, p_end, p_stride), m_obj(p_obj) {
}

template <typename list_t>
CSlice<list_t>::CSlice(const Slice<list_t> &other)
    : SliceBase(other), m_obj(other.m_obj) {}

template <typename list_t>
const typename list_t::elem_t& CSlice<list_t>::operator[](const unsigned idx) const {
  assert(idx < size());
  return m_obj.at(abs_idx(idx));
}

#include <ci_bit_vector.hxx>

template class Slice<CiBitVector>;
template class CSlice<CiBitVector>;

