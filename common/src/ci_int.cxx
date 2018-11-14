#include <ci_int.hxx>

#include <int_op_gen/size.hxx>

using namespace std;
using namespace cingulata;

// template<typename T>
// std::shared_ptr<T> CiInt<T>::int_impl = shared_ptr(new T());
IIntOpGen const* CiInt::m_int_op_gen{ new IntOpGenSize() };


CiInt& CiInt::resize(uint32_t p_bit_cnt) {
  if (is_signed())
    m_bits.resize(p_bit_cnt, msb());
  else
    m_bits.resize(p_bit_cnt, 0);
  return *this;
}

const CiBit CiInt::operator[](const int32_t p_idx) const {
  uint32_t idx = idx_rel_to_abs(p_idx);
  if (idx < size())
    return m_bits.at(idx);
  else if (is_signed())
    return m_bits.back();
  else
    return CiBit(0);
}

CiBit& CiInt::operator[](const int32_t p_idx) {
  uint32_t idx = idx_rel_to_abs(p_idx);
  return m_bits.at(idx);
}

CiInt&  CiInt::operator+=(const CiInt& other) {
  // CiInt::m_int_op_gen->add(*this, other);
  return *this;
}

CiInt&  CiInt::operator-=(const CiInt& other) {
  // CiInt::m_int_op_gen->sub(*this, other);
  return *this;
}

CiInt&  CiInt::operator*=(const CiInt& other) {
  // CiInt::m_int_op_gen->mul(*this, other);
  return *this;
}

CiInt& CiInt::operator&=(const CiInt& other) {
  for (uint32_t i = 0; i < size(); ++i) {
    (*this)[i] &= other[i];
  }
  return *this;
}

CiInt& CiInt::operator|=(const CiInt& other) {
  for (uint32_t i = 0; i < size(); ++i) {
    (*this)[i] |= other[i];
  }
  return *this;
}

CiInt& CiInt::operator^=(const CiInt& other) {
  for (uint32_t i = 0; i < size(); ++i) {
    (*this)[i] ^= other[i];
  }
  return *this;
}

CiInt& CiInt::operator<<=(const int32_t pos) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return (*this) >>= (-pos);

  if (pos > 0) {
    uint32_t ppos = (pos > (int32_t)size()) ? size() : pos;
    m_bits.resize(size() - ppos);
    m_bits.insert(m_bits.begin(), ppos, 0);
  }

  return *this;
}

CiInt& CiInt::operator>>=(const int32_t pos) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return (*this) <<= (-pos);

  if (pos > 0) {
    uint32_t ppos = (pos > (int32_t)size()) ? size() : pos;
    if (is_signed())
      m_bits.resize(size() + ppos, msb());
    else
      m_bits.resize(size() + ppos, 0);
    m_bits.erase(m_bits.begin(), m_bits.begin() + ppos);
  }

  return *this;
}

CiInt& CiInt::rol(const int32_t pos) {
  if (size() > 0 and pos != 0) {
    uint32_t ppos = pos % size(); /**< @c ppos is positive after */

    auto bits_cpy = m_bits;
    m_bits = vector<CiBit>(bits_cpy.end()-ppos, bits_cpy.end());
    m_bits.insert(m_bits.end(), bits_cpy.begin(), bits_cpy.end()-ppos);
  }

  return *this;
}

CiInt& CiInt::ror(const int32_t pos) {
  return ror(-pos);
}


CiInt& CiInt::operator++() {
  *this += 1;
  return *this;
}

CiInt& CiInt::operator--() {
  *this -= 1;
  return *this;
}

CiInt  CiInt::operator++(int) {
  *this += 1;
  return *this;
}

CiInt  CiInt::operator--(int) {
  *this -= 1;
  return *this;
}


template<
  typename T,
  typename = typename std::enable_if<std::is_integral<T>::value, T>::type
>
void CiInt::encode_plain_val(const T p_val, const uint32_t p_bit_cnt) {
  m_bits.clear();
  for (uint32_t i = 0; i < p_bit_cnt; ++i) {
    m_bits.push_back(CiBit((p_val >> i) & 1));
  }
}
