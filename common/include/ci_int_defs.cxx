
template<typename T>
CiInt::CiInt(const T p_val, const unsigned p_bit_cnt, const bool p_is_signed)
:
  m_is_signed(p_is_signed)
{
  encode_plain_val(p_val, p_bit_cnt);
}

template<typename T>
CiInt& CiInt::operator= (const T p_val) {
  encode_plain_val(p_val, size());
  return *this;
}

template<
  typename T,
  typename
>
void CiInt::encode_plain_val(const T p_val, const unsigned p_bit_cnt) {
  m_bits.resize(p_bit_cnt);
  unsigned size_p_val = 8 * sizeof(p_val);
  for (unsigned i = 0; i < p_bit_cnt; ++i) {
    if (i >= size_p_val) {
      m_bits[i] = (p_val >> (size_p_val-1)) & 1;
    } else {
      m_bits[i] = (p_val >> i) & 1;
    }
  }
}
