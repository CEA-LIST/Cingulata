#include <ci_int.hxx>
#include <int_op_gen/size.hxx>

#include <cassert>

using namespace std;
using namespace cingulata;

void CiInt::set_int_op_gen(IIntOpGen const* p_int_op_gen) {
  m_int_op_gen = p_int_op_gen;
}

IIntOpGen const* CiInt::int_op_gen() {
  assert(m_int_op_gen);
  return m_int_op_gen;
}

IIntOpGen const* CiInt::m_int_op_gen{ new IntOpGenSize() };


CiInt::CiInt(const CiBit& p_bit, const bool p_is_signed)
:
  m_is_signed(p_is_signed)
{
  m_bits.push_back(p_bit);
}

CiInt::CiInt(const std::vector<CiBit>& p_bits, const bool p_is_signed)
:
  m_is_signed(p_is_signed)
{
  m_bits = p_bits;
}

CiInt& CiInt::operator= (const CiInt& other) {
  if (this != &other) {
    m_is_signed = other.m_is_signed;
    m_bits = other.m_bits;
  }
  return *this;
}

CiInt::operator CiBit() const {
  return *this != CiInt(0L, size(), is_signed());
}

CiInt CiInt::cast(const unsigned p_bit_cnt, const bool p_is_signed) const {
  return CiInt(*this).alter(p_bit_cnt, p_is_signed);
}

bool CiInt::is_signed() const {
  return m_is_signed;
}

CiInt CiInt::change_sign(const bool p_is_signed) const {
  return CiInt(*this).alter(size(), p_is_signed);
}

CiInt CiInt::to_signed() const {
  return change_sign(true);
}

CiInt CiInt::to_unsigned() const {
  return change_sign(false);
}

unsigned CiInt::size() const {
  return m_bits.size();
}

CiInt CiInt::resize(const unsigned p_bit_cnt) const {
  return CiInt(*this).alter(p_bit_cnt, is_signed());
}

const CiBit& CiInt::operator[](const int p_idx) const {
  unsigned idx = idx_rel_to_abs(p_idx);
  if (idx < size())
    return m_bits.at(idx);
  else
    return sign();
}

CiBit& CiInt::operator[](const int p_idx) {
  unsigned idx = idx_rel_to_abs(p_idx);
  return m_bits.at(idx);
}

CiBit& CiInt::lsb() {
  return (*this)[0];
}

const CiBit& CiInt::lsb() const {
  return (*this)[0];
}

CiBit& CiInt::msb() {
  return (*this)[-1];
}

const CiBit& CiInt::msb() const {
  return (*this)[-1];
}

const CiBit& CiInt::sign() const {
  return is_signed() ? msb() : CiBit::zero;
}

const std::vector<CiBit>& CiInt::bits() const {
  return m_bits;
}

CiInt&  CiInt::operator+=(const CiInt& other) {
  *this = *this + other;
  return *this;
}

CiInt&  CiInt::operator-=(const CiInt& other) {
  *this = *this - other;
  return *this;
}

CiInt&  CiInt::operator*=(const CiInt& other) {
  *this = *this * other;
  return *this;
}

CiInt& CiInt::operator&=(const CiInt& other) {
  for (unsigned i = 0; i < size(); ++i) {
    (*this)[i] &= other[i];
  }
  return *this;
}

CiInt& CiInt::operator|=(const CiInt& other) {
  for (unsigned i = 0; i < size(); ++i) {
    (*this)[i] |= other[i];
  }
  return *this;
}

CiInt& CiInt::operator^=(const CiInt& other) {
  for (unsigned i = 0; i < size(); ++i) {
    (*this)[i] ^= other[i];
  }
  return *this;
}

CiInt& CiInt::operator<<=(const int pos) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return (*this) >>= (-pos);

  if (pos > 0) {
    unsigned ppos = (pos > (int)size()) ? size() : pos;
    m_bits.resize(size() - ppos);
    m_bits.insert(m_bits.begin(), ppos, CiBit::zero);
  }

  return *this;
}

CiInt& CiInt::operator>>=(const int pos) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return (*this) <<= (-pos);

  if (pos > 0) {
    unsigned ppos = (pos > (int)size()) ? size() : pos;
    if (is_signed())
      m_bits.resize(size() + ppos, msb());
    else
      m_bits.resize(size() + ppos, CiBit::zero);
    m_bits.erase(m_bits.begin(), m_bits.begin() + ppos);
  }

  return *this;
}

CiInt& CiInt::rol(const int pos) {
  if (size() > 0 and pos != 0) {
    unsigned ppos = pos % size(); /**< @c ppos is positive after */

    auto bits_cpy = m_bits;
    m_bits = vector<CiBit>(bits_cpy.end()-ppos, bits_cpy.end());
    m_bits.insert(m_bits.end(), bits_cpy.begin(), bits_cpy.end()-ppos);
  }

  return *this;
}

CiInt& CiInt::ror(const int pos) {
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

CiInt CiInt::operator++(int) {
  CiInt cpy{*this};
  --(*this);
  return cpy;
}

CiInt CiInt::operator--(int) {
  CiInt cpy{*this};
  --(*this);
  return cpy;
}

CiInt& CiInt::alter(const unsigned p_bit_cnt, const bool p_is_signed) {
  m_bits.resize(p_bit_cnt, sign());
  m_is_signed = p_is_signed;
  return *this;
}

unsigned CiInt::idx_rel_to_abs(const int idx) const {
  return (idx < 0) ? size()+idx : idx;
}

/* Logical operators */
CiBit operator!(const CiInt& lhs) {
  return !static_cast<CiBit>(lhs);
}

CiBit operator&&(const CiInt& lhs, const CiInt& rhs) {
  return static_cast<CiBit>(lhs) & static_cast<CiBit>(rhs);
}
CiBit operator||(const CiInt& lhs, const CiInt& rhs) {
  return static_cast<CiBit>(lhs) | static_cast<CiBit>(rhs);
}

CiInt operator+(const CiInt& lhs) {
  return lhs;
}

CiInt operator-(const CiInt& lhs) {
  return (~lhs)+CiBit(1); /** < Two's complement negation */
}

/**
 * @brief      Gets the bit-size the output of a 2 input operator.
 * @details    This function computes the bit-size of the integer obtained as a
 *             result of application of an 2-input integer operator.
 *
 * @param[in]  lhs   The left hand side
 * @param[in]  rhs   The right hand side
 *
 * @return     Result bit-size
 */
unsigned result_size(const CiInt& lhs, const CiInt& rhs) {
  return max(lhs.size(), rhs.size());
}

/**
 * @brief      Gets the sign the output of a 2 input operator.
 * @details    This function computes the signedness of the integer obtained as
 *             a result of application of an 2-input integer operator.
 *
 * @param[in]  lhs   The left hand side
 * @param[in]  rhs   The right hand side
 *
 * @return     Result sign
 */
bool result_sign(const CiInt& lhs, const CiInt& rhs) {
  if (lhs.size() == rhs.size()) {
    return lhs.is_signed() and rhs.is_signed();
  } else if (lhs.size() > rhs.size()) {
    return lhs.is_signed();
  } else {
    return rhs.is_signed();
  }
}

/* Arithmetic operators */
CiInt operator+(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same multiply by 2 */
    return lhs.cast(res_size, res_size) << 1;
  } else {
    auto res_bits = CiInt::int_op_gen()->add(lhs.resize(res_size).bits(), rhs.resize(res_size).bits());
    return CiInt(res_bits, res_is_signed);
  }
}

CiInt operator-(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  IIntOpGen::BitVectorT res_bits;
  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same the difference is zero */
    res_bits.resize(res_size, 0);
  } else {
    res_bits = CiInt::int_op_gen()->sub(lhs.resize(res_size).bits(), rhs.resize(res_size).bits());
  }
  return CiInt(res_bits, res_is_signed);
}

CiInt operator*(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  IIntOpGen::BitVectorT res_bits;
  if (&lhs == &rhs) {
    /** < when @c lhs and @c rhs are the same square @c lhs */
    res_bits = CiInt::int_op_gen()->square(lhs.bits());
  } else {
    res_bits = CiInt::int_op_gen()->mul(lhs.resize(res_size).bits(), rhs.resize(res_size).bits());
  }
  return CiInt(res_bits, res_is_signed);
}

// CiInt CiInt::operator/(const CiInt& lhs, const CiInt& rhs);
// CiInt CiInt::operator%(const CiInt& lhs, const CiInt& rhs);

/* Bitwise logic */
CiInt operator~(CiInt lhs) {
  for (unsigned i = 0; i < lhs.size(); ++i) {
    lhs[i] = ~lhs[i];
  }
  return lhs;
}

#define DEFINE_BITWISE_OPERATOR(OP_NAME, OP, SAME_OPERANDS_CODE) \
CiInt cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  CiInt res{lhs.cast(result_size(lhs, rhs), result_sign(lhs, rhs))}; \
 \
  if (&lhs == &rhs) { \
    SAME_OPERANDS_CODE; \
  } else { \
    for (unsigned i = 0; i < res.size(); ++i) { \
      res[i] OP rhs[i]; \
    } \
  } \
  return res; \
}

DEFINE_BITWISE_OPERATOR(operator&, &=,        );
DEFINE_BITWISE_OPERATOR(operator|, |=,        );
DEFINE_BITWISE_OPERATOR(operator^, ^=, res = 0);

/* Bitwise shift */
CiInt operator<<(CiInt lhs, const unsigned& pos) {
  return lhs <<= pos;
}

CiInt operator>>(CiInt lhs, const unsigned& pos) {
  return lhs >>= pos;
}

CiInt rol(CiInt lhs, const unsigned& pos) {
  return lhs.rol(pos);
}

CiInt ror(CiInt lhs, const unsigned& pos) {
  return lhs.ror(pos);
}

/* Relational operators */
#define DEFINE_RELATIONAL_OPERATOR(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE) \
CiBit OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  if (&lhs == &rhs) {  \
    SAME_OPERANDS_CODE; \
  } else { \
    unsigned res_size = result_size(lhs, rhs); \
    return CiInt::int_op_gen()->OP_FUNC(lhs.resize(res_size).bits(), rhs.resize(res_size).bits()); \
  } \
}

DEFINE_RELATIONAL_OPERATOR(operator==, equal         , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR(operator!=, not_equal     , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator< , lower         , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator<=, lower_equal   , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR(operator> , greater       , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator>=, greater_equal , return CiBit::one);
