
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
  //  typename = typename std::enable_if<std::is_integral<T>::value, T>::type
>
void CiInt::encode_plain_val(const T p_val, const unsigned p_bit_cnt) {
  m_bits.clear();
  unsigned int size_p_val = 8 * sizeof(p_val);
  for (unsigned int i = 0; i < p_bit_cnt; ++i) {
    if (i >= size_p_val) {
      m_bits.push_back(CiBit((p_val >> (size_p_val-1)) & 1));
    }
    else {
      m_bits.push_back(CiBit((p_val >> i) & 1));
    }
  }
}
