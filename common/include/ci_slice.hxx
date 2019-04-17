#ifndef CI_SLICE
#define CI_SLICE

#include <ci_bit.hxx>

#include <vector>
#include <functional>
#include <optional>

namespace cingulata {

template<typename T>
class Slice {
public:
  Slice(const T& p_obj, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  Slice(const Slice& slice) = default;

  unsigned size() const;

  CiBit& operator[](const int idx);
  const CiBit& operator[](const int idx) const;

  /**
   * @brief      Assign @c other bits to bits pointed by current slice
   * @details    In case current and @c other slice sizes differ then only the
   *             first @c min(size(), other.size()) are assigned.
   *
   * @param[in]  other  slice to assign from
   *
   * @return     reference to current object
   */
   Slice& operator=(const Slice& other);


private:
  std::vector<std::reference_wrapper<const CiBit>> m_bits;
};

#include "ci_slice-impl.hxx"

} // namespace cingulata

#endif
