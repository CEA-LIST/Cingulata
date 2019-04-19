#ifndef CI_SLICE
#define CI_SLICE

#include <ci_bit.hxx>

#include <vector>
#include <functional>
#include <optional>

namespace cingulata {

class SliceBase {
public:
  SliceBase(const int p_list_size, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  unsigned size() const;
  unsigned map_idx(const unsigned idx) const;
protected:
  int m_stride;
  int m_start;
  int m_end;
  unsigned m_size;
};

template<typename list_t>
class CSlice;

template<typename list_t>
class Slice : public SliceBase {
  friend class CSlice<list_t>;
public:
  Slice(list_t& p_obj, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  typename list_t::elem_t& operator[](const unsigned idx);
  const typename list_t::elem_t& operator[](const unsigned idx) const;

  Slice& operator=(const Slice& other);
  Slice& operator=(const CSlice<list_t>& other);

protected:
  list_t& m_obj;
};

template<typename list_t>
class CSlice : public SliceBase {
public:
  CSlice(const list_t& p_obj, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  CSlice(const Slice<list_t>& other);

  const typename list_t::elem_t& operator[](const unsigned idx) const;

protected:
  const list_t& m_obj;
};

} // namespace cingulata

#endif
