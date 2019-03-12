#include <ci_int.hxx>
#include <ci_context.hxx>

#include <cassert>

using namespace std;
using namespace cingulata;

/**
 * Define name manager format to use for @c CiInt types
 */
template<>
string IoNameVec<CiInt>::m_name_fmt{ "%s_%d" };

const CiInt CiInt::u8   {0,  8, false};
const CiInt CiInt::u16  {0, 16, false};
const CiInt CiInt::u32  {0, 32, false};
const CiInt CiInt::u64  {0, 64, false};

const CiInt CiInt::s8   {0,  8, true };
const CiInt CiInt::s16  {0, 16, true };
const CiInt CiInt::s32  {0, 32, true };
const CiInt CiInt::s64  {0, 64, true };

CiInt::CiInt(const CiBit& p_bit, const unsigned p_bit_cnt, const bool p_is_signed)
:
  m_bits(p_bit_cnt, p_bit),
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
    m_bits = other.cast(m_bits.size());
  }
  return *this;
}

CiInt::operator CiBit() const {
  return *this != CiInt(0L, size(), is_signed());
}

CiInt::operator CiBitVector() const {
  return cast();
}

CiBitVector CiInt::cast(const unsigned p_bit_cnt) const {
  CiBitVector tmp = cast();
  tmp.resize(p_bit_cnt, sign());
  return tmp;
}

CiBitVector CiInt::cast() const {
  return m_bits;
}

bool CiInt::is_signed() const {
  return m_is_signed;
}

CiInt CiInt::change_sign(const bool p_is_signed) const {
  return alter(size(), p_is_signed);
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
  return alter(p_bit_cnt, is_signed());
}

CiInt CiInt::alter(const unsigned p_bit_cnt, const bool p_is_signed) const {
  return CiInt(cast(p_bit_cnt), p_is_signed);
}

const CiBit& CiInt::operator[](const int p_idx) const {
  return m_bits.at(p_idx, sign());
}

CiBit& CiInt::operator[](const int p_idx) {
  return m_bits.at(p_idx);
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
  return is_signed() ? m_bits.at(-1) : CiBit::zero;
}

CiInt CiInt::operator+() const {
  return *this;
}

CiInt CiInt::operator-() const {
  return CiInt(CiContext::get_int_op_gen()->neg(cast()), is_signed());
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
  m_bits.shr(pos, CiBit::zero);
  return *this;
}

CiInt& CiInt::operator>>=(const int pos) {
  m_bits.shl(pos, sign());
  return *this;
}

CiInt& CiInt::rol(const int pos) {
  m_bits.ror(pos);
  return *this;
}

CiInt& CiInt::ror(const int pos) {
  m_bits.rol(pos);
  return *this;
}


CiInt& CiInt::operator++() {
  *this += CiBit::one;
  return *this;
}

CiInt& CiInt::operator--() {
  *this -= CiBit::one;
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

namespace
{
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
  bool result_is_signed(const CiInt& lhs, const CiInt& rhs) {
    if (lhs.size() == rhs.size()) {
      return lhs.is_signed() and rhs.is_signed();
    } else if (lhs.size() > rhs.size()) {
      return lhs.is_signed();
    } else {
      return rhs.is_signed();
    }
  }
}

/* Arithmetic operators */
CiInt cingulata::operator+(const CiInt& lhs, const CiInt& rhs) {
  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same multiply by 2 */
    return lhs << 1;
  } else {
    unsigned res_size = result_size(lhs, rhs);
    bool res_is_signed = result_is_signed(lhs, rhs);

    auto res = CiContext::get_int_op_gen()->add(
      lhs.cast(res_size),
      rhs.cast(res_size));
    return CiInt(res, res_is_signed);
  }
}

CiInt cingulata::operator-(const CiInt& lhs, const CiInt& rhs) {
  if (&lhs == &rhs) {
     /** < when @c lhs and @c rhs are the same the difference is zero */
    return CiInt(CiBit::zero, lhs.size(), lhs.is_signed());
  } else {
    unsigned res_size = result_size(lhs, rhs);
    bool res_is_signed = result_is_signed(lhs, rhs);

    auto res = CiContext::get_int_op_gen()->sub(
      lhs.cast(res_size),
      rhs.cast(res_size));
    return CiInt(res, res_is_signed);
  }
}

CiInt cingulata::operator*(const CiInt& lhs, const CiInt& rhs) {
  if (&lhs == &rhs) {
    /** < when @c lhs and @c rhs are the same square @c lhs */
    return CiInt(CiContext::get_int_op_gen()->square(lhs.cast()), lhs.is_signed());
  } else {
    unsigned res_size = result_size(lhs, rhs);
    bool res_is_signed = result_is_signed(lhs, rhs);

    auto res = CiContext::get_int_op_gen()->mul(
      lhs.cast(res_size),
      rhs.cast(res_size));
    return CiInt(res, res_is_signed);
  }
}

// CiInt CiInt::operator/(const CiInt& lhs, const CiInt& rhs);
// CiInt CiInt::operator%(const CiInt& lhs, const CiInt& rhs);

/* Bitwise logic */
CiInt cingulata::operator~(const CiInt& lhs) {
  return lhs.cast().op_not(); // TODO: see if unittest catch this bug
  // return CiInt(lhs.cast().op_not(), lhs.is_signed());
}

#define DEFINE_BITWISE_OPERATOR(OP_NAME, OP, SAME_OPERANDS_CODE) \
CiInt cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  CiInt res { lhs.alter(result_size(lhs, rhs), result_is_signed(lhs, rhs)) }; \
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
CiInt cingulata::operator<<(CiInt lhs, const int pos) {
  return lhs <<= pos;
}

CiInt cingulata::operator>>(CiInt lhs, const int pos) {
  return lhs >>= pos;
}

CiInt cingulata::rol(CiInt lhs, const int pos) {
  return lhs.rol(pos);
}

CiInt cingulata::ror(CiInt lhs, const int pos) {
  return lhs.ror(pos);
}

/* Relational operators */
#define DEFINE_RELATIONAL_OPERATOR(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE) \
CiBit cingulata::OP_NAME(const CiInt& lhs, const CiInt& rhs) { \
  if (&lhs == &rhs) {  \
    SAME_OPERANDS_CODE; \
  } else { \
    unsigned res_size = result_size(lhs, rhs); \
    return CiContext::get_int_op_gen()->OP_FUNC( \
      lhs.cast(res_size), \
      rhs.cast(res_size)); \
  } \
}

DEFINE_RELATIONAL_OPERATOR(operator==, equal         , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR(operator!=, not_equal     , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator< , lower         , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator<=, lower_equal   , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR(operator> , greater       , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR(operator>=, greater_equal , return CiBit::one);


istream& cingulata::operator>>(istream& inp, CiInt& val) {
  val.read();
  return inp;
}

ostream& cingulata::operator<<(ostream& out, CiInt& val) {
  val.write();
  return out;
}

