#include <slice.hxx>

#include <cassert>

using namespace std;
using namespace cingulata;

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

unsigned SliceBase::map_idx(const unsigned idx) const {
  return m_start + idx * m_stride;
}

template <typename list_t>
Slice<list_t>::Slice(list_t& p_obj, const optional<int> &p_start,
                     const optional<int> &p_end, const optional<int> &p_stride)
    : SliceBase(p_obj.size(), p_start, p_end, p_stride), m_obj(p_obj) {
}

template <typename list_t>
typename list_t::elem_t& Slice<list_t>::operator[](const unsigned idx) {
  assert(idx < size());
  return m_obj.at(map_idx(idx));
}

template <typename list_t>
const typename list_t::elem_t& Slice<list_t>::operator[](const unsigned idx) const {
  assert(idx < size());
  return m_obj.at(map_idx(idx));
}

template <typename list_t>
Slice<list_t>& Slice<list_t>::operator=(const Slice& other) {
  assert(size() == other.size());
  for (int i = 0; i < (int)size(); ++i) {
    (*this)[i] = other[i];
  }
  return *this;
}

template <typename list_t>
Slice<list_t>& Slice<list_t>::operator=(const CSlice<list_t>& other) {
  assert(size() == other.size());
  for (int i = 0; i < (int)size(); ++i) {
    (*this)[i] = other[i];
  }
  return *this;
}


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
  return m_obj.at(map_idx(idx));
}

#include <ci_bit_vector.hxx>

template class Slice<CiBitVector>;
template class CSlice<CiBitVector>;

