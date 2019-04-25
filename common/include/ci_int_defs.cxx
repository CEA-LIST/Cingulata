
template<
  typename T,
  typename
>
CiInt::CiInt(const T p_val, const unsigned p_bit_cnt, const bool p_is_signed)
:
  m_is_signed(p_is_signed)
{
  encode_plain_val(p_val, p_bit_cnt);
}

template<
  typename T,
  typename
>
CiInt& CiInt::operator= (const T p_val) {
  encode_plain_val(p_val, size());
  return *this;
}

template<
  typename T,
  typename
>
void CiInt::encode_plain_val(const T p_val, const unsigned p_bit_cnt) {
  m_bits.resize(0);
  for (unsigned i = 0; i < 8 * sizeof(p_val); ++i) {
    m_bits.append((p_val >> i) & 1);
  }

  unsigned bit_cnt = p_bit_cnt;
  if (p_bit_cnt == (unsigned)-1) {
    T val = p_val >= 0 ? p_val : -p_val;
    bit_cnt = (unsigned)std::ceil(std::log2(double(val)+1)); //number of bits for p_val
    if (p_val != val) bit_cnt++; //aditional bit for the sign
  }

  m_bits.resize(bit_cnt, sign());
}
