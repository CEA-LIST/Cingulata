#ifndef UTILS
#define UTILS

#include <cassert>
#include <cmath>
#include <vector>

namespace cingulata {

/**
 * type for plain-text bit
 */
typedef unsigned bit_plain_t;

/**
 * @brief      Encodes an integral native type value into a vector of bits.
 * @details    This methods encodes an integer value into a bit-vector starting
 *             with the LSB (i.e. 1st LSB of @c p_val on position 0, 2nd LSB on
 *             position 1, etc.). Output bit-vector size:
 *              * if @c p_bit_cnt!=-1 the obtained bit-vector is extended with
 *                zeros if @c p_val>=0 and with ones otherwise,
 *              * if @c p_bit_cnt==-1 the bit-vector is truncated to minimal
 *                number of bits. There are 2 possibilities as a function of the
 *                sign of @c p_val:
 *                 * @c p_val>=0 -- all leading zero bits are truncated,
 *                 * @c p_val<0 -- all except last leading one bits are
 *                   truncated.
 *
 * @param      p_val      input value
 * @param      p_bit_cnt  number of bits
 *
 * @tparam     int_t      native integral type
 * @tparam     bit_t      output bit type
 * @tparam     <unnamed>  only integral types are accepted
 *
 * @return     encoding of input value into bits
 */
template <typename int_t,
          typename = typename std::enable_if<std::is_integral<int_t>::value,
                                             int_t>::type>
std::vector<bit_plain_t> encode_plain_int(const int_t p_val,
                                    const unsigned p_bit_cnt = -1) {

  const bool is_negative = (p_val < 0);
  const int_t val = is_negative ? -p_val : p_val;
  const unsigned min_bit_cnt = (unsigned)std::ceil(
      std::log2(double(val) + 1)); // minimal number of bits for p_val

  std::vector<bit_plain_t> result;
  for (unsigned i = 0; i < min_bit_cnt; ++i) {
    result.push_back((p_val >> i) & 1);
  }

  if (p_bit_cnt == (unsigned)-1) {
    if (is_negative)
      result.push_back(bit_plain_t(1));
  } else {
    result.resize(p_bit_cnt, bit_plain_t(is_negative));
  }

  return result;
}

} // namespace cingulata
#endif
