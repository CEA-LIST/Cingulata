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


CiInt CiInt::u8v(const uint8_t p_val)    { return CiInt(p_val, 8, false); }
CiInt CiInt::u16v(const uint16_t p_val)  { return CiInt(p_val, 16, false); }
CiInt CiInt::u32v(const uint32_t p_val)  { return CiInt(p_val, 32, false); }
CiInt CiInt::u64v(const uint64_t p_val)  { return CiInt(p_val, 64, false); }

CiInt CiInt::s8v(const int8_t p_val)     { return CiInt(p_val, 8, true); }
CiInt CiInt::s16v(const int16_t p_val)   { return CiInt(p_val, 16, true); }
CiInt CiInt::s32v(const int32_t p_val)   { return CiInt(p_val, 32, true); }
CiInt CiInt::s64v(const int64_t p_val)   { return CiInt(p_val, 64, true); }

const CiInt CiInt::u8   (CiInt::u8v(0));
const CiInt CiInt::u16  (CiInt::u16v(0));
const CiInt CiInt::u32  (CiInt::u32v(0));
const CiInt CiInt::u64  (CiInt::u64v(0));

const CiInt CiInt::s8   (CiInt::s8v(0));
const CiInt CiInt::s16  (CiInt::s16v(0));
const CiInt CiInt::s32  (CiInt::s32v(0));
const CiInt CiInt::s64  (CiInt::s64v(0));

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

CiInt &CiInt::operator=(const int64_t p_val) {
  m_bits = encode_plain_int(p_val, size());
  return *this;
}

CiInt::operator CiBit() const {
  return *this != CiInt(CiBit::zero, size(), is_signed());
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
  if (pos < 0)
    m_bits.shl(-pos, sign());
  else
    m_bits.shr(pos, CiBit::zero);
  return *this;
}

CiInt& CiInt::operator>>=(const int pos) {
  if (pos < 0)
    m_bits.shr(-pos, CiBit::zero);
  else
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
unsigned cingulata::result_size(const CiInt& lhs, const CiInt& rhs) {
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
bool cingulata::result_is_signed(const CiInt& lhs, const CiInt& rhs) {
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

/* Equal/not-equal operators */
#define DEFINE_RELATIONAL_OPERATOR_1(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE)     \
  CiBit cingulata::OP_NAME(const CiInt &lhs, const CiInt &rhs) {               \
    if (&lhs == &rhs) {                                                        \
      SAME_OPERANDS_CODE;                                                      \
    } else {                                                                   \
      unsigned res_size = result_size(lhs, rhs);                               \
      return CiContext::get_int_op_gen()->OP_FUNC(lhs.cast(res_size),          \
                                                  rhs.cast(res_size));         \
    }                                                                          \
  }

DEFINE_RELATIONAL_OPERATOR_1(operator==, equal         , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR_1(operator!=, not_equal     , return CiBit::zero);

/* Comparators */
#define DEFINE_RELATIONAL_OPERATOR_2(OP_NAME, OP_FUNC, SAME_OPERANDS_CODE)     \
  CiBit cingulata::OP_NAME(const CiInt &lhs, const CiInt &rhs) {               \
    if (&lhs == &rhs) {                                                        \
      SAME_OPERANDS_CODE;                                                      \
    } else {                                                                   \
      unsigned res_size = result_size(lhs, rhs);                               \
      CiBit result = CiContext::get_int_op_gen()->OP_FUNC(lhs.cast(res_size),  \
                                                          rhs.cast(res_size)); \
      if (lhs.is_signed() or rhs.is_signed())                                  \
        result ^= lhs.sign() ^ rhs.sign();                                     \
      return result;                                                           \
    }                                                                          \
  }

DEFINE_RELATIONAL_OPERATOR_2(operator< , lower         , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR_2(operator<=, lower_equal   , return CiBit::one);
DEFINE_RELATIONAL_OPERATOR_2(operator> , greater       , return CiBit::zero);
DEFINE_RELATIONAL_OPERATOR_2(operator>=, greater_equal , return CiBit::one);

istream& cingulata::operator>>(istream& inp, CiInt& val) {
  val.read();
  return inp;
}

ostream& cingulata::operator<<(ostream& out, const CiInt& val) {
  val.write();
  return out;
}

