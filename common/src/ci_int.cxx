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

IIntOpGen const* CiInt::m_int_op_gen{ nullptr };

CiInt::CiInt(const CiBit& p_bit, const unsigned p_bit_cnt, const bool p_is_signed)
:
  m_bits(p_bit_cnt, p_bit),
  m_is_signed(p_is_signed)
{}

CiInt::CiInt(const initializer_list<CiBit>& p_bits, const bool p_is_signed)
:
  m_bits(p_bits),
  m_is_signed(p_is_signed)
{}

CiInt::CiInt(const vector<CiBit>& p_bits, const bool p_is_signed)
:
  m_bits(p_bits),
  m_is_signed(p_is_signed)
{}

CiInt::CiInt(const CiBitVector& p_bits, const bool p_is_signed)
:
  m_bits(p_bits),
  m_is_signed(p_is_signed)
{}

CiInt& CiInt::operator= (const CiInt& other) {
  if (this != &other) {
    m_bits = other.m_bits;
    m_is_signed = other.m_is_signed;
  }
  return *this;
}

CiInt::operator CiBit() const {
  return *this != CiInt(0L, size(), is_signed());
}

CiInt::operator CiBitVector() {
  return m_bits;
}

CiBitVector& CiInt::bits() {
  return m_bits;
}

const CiBitVector& CiInt::bits() const {
  return m_bits;
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
  return m_bits.at(p_idx, sign());
}

CiBit& CiInt::operator[](const int p_idx) {
  return m_bits.at(p_idx);
}

CiBit& CiInt::lsb() {
  return (*this)[-1];
}

const CiBit& CiInt::lsb() const {
  return (*this)[-1];
}

CiBit& CiInt::msb() {
  return (*this)[0];
}

const CiBit& CiInt::msb() const {
  return (*this)[0];
}

const CiBit& CiInt::sign() const {
  return is_signed() ? msb() : CiBit::zero;
}

CiInt& CiInt::operator+=(const CiInt& other) {
  *this = *this + other;
  return *this;
}

CiInt& CiInt::operator-=(const CiInt& other) {
  *this = *this - other;
  return *this;
}

CiInt& CiInt::operator*=(const CiInt& other) {
  *this = *this * other;
  return *this;
}

CiInt& CiInt::operator&=(const CiInt& other) {
  m_bits.op_and(other.m_bits, other.sign());
  return *this;
}

CiInt& CiInt::operator|=(const CiInt& other) {
  m_bits.op_or(other.m_bits, other.sign());
  return *this;
}

CiInt& CiInt::operator^=(const CiInt& other) {
  m_bits.op_xor(other.m_bits, other.sign());
  return *this;
}

CiInt& CiInt::operator<<=(const int pos) {
  bits().shl(pos, CiBit::zero);
  return *this;
}

CiInt& CiInt::operator>>=(const int pos) {
  bits().shr(pos, sign());
  return *this;
}

CiInt& CiInt::rol(const int pos) {
  bits().rol(pos);
  return *this;
}

CiInt& CiInt::ror(const int pos) {
  bits().ror(pos);
  return *this;
}


CiInt& CiInt::operator++() {
  *this += CiBit(1);
  return *this;
}

CiInt& CiInt::operator--() {
  *this -= CiBit(1);
  return *this;
}

CiInt CiInt::operator++(int) {
  CiInt cpy{*this};
  ++(*this);
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

/* Logical operators */
CiBit cingulata::operator!(const CiInt& lhs) {
  return !static_cast<CiBit>(lhs);
}

CiBit cingulata::operator&&(const CiInt& lhs, const CiInt& rhs) {
  return static_cast<CiBit>(lhs) & static_cast<CiBit>(rhs);
}
CiBit cingulata::operator||(const CiInt& lhs, const CiInt& rhs) {
  return static_cast<CiBit>(lhs) | static_cast<CiBit>(rhs);
}

CiInt cingulata::operator+(const CiInt& lhs) {
  return lhs;
}

CiInt cingulata::operator-(const CiInt& lhs) {
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
  return (lhs.size() > rhs.size()) ? lhs.size() : rhs.size();
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
CiInt cingulata::operator+(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same multiply by 2 */
    return lhs.cast(res_size, res_size) << 1;
  } else {
    auto res = CiInt::int_op_gen()->add(
      lhs.resize(res_size).bits(),
      rhs.resize(res_size).bits());
    return CiInt(res, res_is_signed);
  }
}

CiInt cingulata::operator-(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  CiBitVector res;
  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same the difference is zero */
    res = CiBitVector(res_size, CiBit::zero);
  } else {
    res = CiInt::int_op_gen()->sub(
      lhs.resize(res_size).bits(),
      rhs.resize(res_size).bits());
  }
  return CiInt(res, res_is_signed);
}

CiInt cingulata::operator*(const CiInt& lhs, const CiInt& rhs) {
  unsigned res_size = result_size(lhs, rhs);
  bool res_is_signed = result_sign(lhs, rhs);

  CiBitVector res;
  if (&lhs == &rhs) {
    /** < when @c lhs and @c rhs are the same square @c lhs */
    res = CiInt::int_op_gen()->square(lhs.bits());
  } else {
    res = CiInt::int_op_gen()->mul(
      lhs.resize(res_size).bits(),
      rhs.resize(res_size).bits());
  }
  return CiInt(res, res_is_signed);
}

// CiInt CiInt::operator/(const CiInt& lhs, const CiInt& rhs);
// CiInt CiInt::operator%(const CiInt& lhs, const CiInt& rhs);

/* Bitwise logic */
CiInt cingulata::operator~(CiInt lhs) {
  lhs.bits().op_not();
  return lhs;
}

#define DEFINE_BITWISE_OPERATOR(OP_NAME, OP, SAME_OPERANDS_CODE) \
CiInt cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  CiInt res{lhs.cast(result_size(lhs, rhs), result_sign(lhs, rhs))}; \
 \
  if (&lhs == &rhs) { \
    SAME_OPERANDS_CODE; \
  } else { \
    res OP rhs; \
  } \
  return res; \
}

DEFINE_BITWISE_OPERATOR(operator&, &=,        );
DEFINE_BITWISE_OPERATOR(operator|, |=,        );
DEFINE_BITWISE_OPERATOR(operator^, ^=, res = 0);

/* Bitwise shift */
CiInt cingulata::operator<<(CiInt lhs, const unsigned& pos) {
  return lhs <<= pos;
}

CiInt cingulata::operator>>(CiInt lhs, const unsigned& pos) {
  return lhs >>= pos;
}

CiInt cingulata::rol(CiInt lhs, const unsigned& pos) {
  return lhs.rol(pos);
}

CiInt cingulata::ror(CiInt lhs, const unsigned& pos) {
  return lhs.ror(pos);
}

/* Relational operators */
#define DEFINE_RELATIONAL_OPERATOR_1(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE) \
CiBit cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  if (&lhs == &rhs) {  \
    SAME_OPERANDS_CODE; \
  } else { \
    unsigned res_size = result_size(lhs, rhs); \
    return CiInt::int_op_gen()->OP_FUNC( \
      lhs.resize(res_size).bits(), \
      rhs.resize(res_size).bits()); \
  } \
}

#define DEFINE_RELATIONAL_OPERATOR_2(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE) \
CiBit cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  if (&lhs == &rhs) {  \
    SAME_OPERANDS_CODE; \
  } else { \
    unsigned res_size = result_size(lhs, rhs); \
    CiBit res = CiInt::int_op_gen()->OP_FUNC( \
      lhs.resize(res_size).bits(), \
      rhs.resize(res_size).bits()); \
    bool res_is_signed = result_sign(lhs, rhs); \
    if (res_is_signed) \
      res ^= lhs.sign() ^ rhs.sign(); \
    return res; \
  } \
}

DEFINE_RELATIONAL_OPERATOR_1(operator==, equal         , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR_1(operator!=, not_equal     , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR_2(operator< , lower         , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR_2(operator<=, lower_equal   , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR_2(operator> , greater       , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR_2(operator>=, greater_equal , return CiBit::one);
