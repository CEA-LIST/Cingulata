#include <cassert>

template <typename T>
Slice<T>::Slice(const T& p_obj, const std::optional<int>& p_start, const std::optional<int>& p_end,
      const std::optional<int>& p_stride) {
  int stride;

  stride = p_stride.value_or(1);
  assert(stride != 0);

  int size = p_obj.size();
  if (stride > 0) {
    int start = p_start.value_or(0);
    int end = p_end.value_or(size);
    for (int i = start; i < end; i += stride) {
      m_bits.emplace_back(p_obj.at(i));
    }
  } else {
    int start = p_start.value_or(size-1);
    int end = p_end.value_or(-1);
    for (int i = start; i > end; i += stride) {
      m_bits.emplace_back(p_obj.at(i));
    }
  }
}

template<typename T>
unsigned Slice<T>::size() const {
  return m_bits.size();
}

template<typename T>
CiBit& Slice<T>::operator[](const int idx) {
  assert(idx >= 0 and idx < size());
  return m_bits[idx].get();
}

template<typename T>
const CiBit& Slice<T>::operator[](const int idx) const {
  assert(idx >= 0 and idx < (int)size());
  return m_bits[idx].get();
}

template<typename T>
Slice<T>& Slice<T>::operator=(const Slice<T>& other) {
  assert(size() == other.size());
  for (int i = 0; i < size(); ++i) {
    m_bits[i] = other.m_bits[i];
  }
  return *this;
}

