
template <typename T, typename>
CiInt::CiInt(const T p_val, const unsigned p_bit_cnt, const bool p_is_signed)
    : m_bits(encode_plain_int(p_val, p_bit_cnt)),
      m_is_signed(p_is_signed) {}

template <typename T, typename> CiInt &CiInt::operator=(const T p_val) {
  m_bits = encode_plain_int(p_val, size());
  return *this;
}
