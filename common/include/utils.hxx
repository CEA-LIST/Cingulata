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

#ifndef UTILS
#define UTILS

#include <cmath>
#include <cstdint>
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

template <typename int_t = uint64_t,
          typename = typename std::enable_if<std::is_integral<int_t>::value,
                                             int_t>::type>
int_t decode_plain_int(const std::vector<bit_plain_t>& p_bit_vec) {
  int_t val = 0;
  const size_t n = std::min(p_bit_vec.size(), sizeof(uint64_t)*8);
  for (size_t i = 0; i < n; ++i)
    val |= p_bit_vec[i] << i;
  return val;
}

} // namespace cingulata
#endif
