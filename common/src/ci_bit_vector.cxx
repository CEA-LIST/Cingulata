#include <ci_bit_vector.hxx>
#include <iostream>
#include <cassert>

using namespace std;
using namespace cingulata;


/**
 * Define name manager format to use for @c CiBitVector types
 */
template<>
string IoNameVec<CiBitVector>::m_name_fmt{ "%s_%d" };

CiBitVector::CiBitVector(
    const int p_bit_cnt,
    const CiBit& p_bit)
:
  m_vec(p_bit_cnt, p_bit)
{}

CiBitVector::CiBitVector(const initializer_list<CiBit>& p_bits)
:
  m_vec(p_bits)
{}

CiBitVector::CiBitVector(const vector<CiBit>& p_bits)
:
  m_vec(p_bits)
{}

CiBitVector& CiBitVector::operator= (const CiBitVector& other) {
  if (this != &other) {
    m_vec = other.m_vec;
  }
  return *this;
}

unsigned CiBitVector::size() const {
  return m_vec.size();
}

CiBitVector& CiBitVector::resize(const unsigned p_bit_cnt, const CiBit& p_bit) {
  m_vec.resize(p_bit_cnt, p_bit);
  return *this;
}

CiBitVector& CiBitVector::append(const CiBit& p_bit) {
  m_vec.push_back(p_bit);
  return *this;
}

CiBit& CiBitVector::operator[](const int p_idx) {
  return at(p_idx);
}

const CiBit& CiBitVector::operator[](const int p_idx) const {
  return at(p_idx, CiBit::zero);
}

CiBit& CiBitVector::at(const int p_idx) {
  unsigned idx = idx_rel_to_abs(p_idx);
  return m_vec.at(idx);
}

const CiBit& CiBitVector::at(const int p_idx, const CiBit& p_bit) const {
  unsigned idx = idx_rel_to_abs(p_idx);
  if (idx < size())
    return m_vec.at(idx);
  else
    return p_bit;
}

CiBitVector CiBitVector::slice(int p_start, int p_end, const int p_stride) const {
  p_start = idx_clip(idx_rel_to_abs(p_start));
  p_end = idx_clip(idx_rel_to_abs(p_end));

  vector<CiBit> bits;

  if (p_stride > 0) {
    for (int i = p_start; i < (int)p_end; i += p_stride)
      bits.push_back(m_vec.at(i));
  } else if (p_stride < 0) {
    for (int i = p_start; i > (int)p_end; i += p_stride)
      bits.push_back(m_vec.at(i));
  }

  return bits;
}

CiBitVector& CiBitVector::operator&=(const CiBitVector& other) {
  this->op_and(other, CiBit::one);
  return *this;
}

CiBitVector& CiBitVector::operator|=(const CiBitVector& other) {
  this->op_or(other, CiBit::zero);
  return *this;
}

CiBitVector& CiBitVector::operator^=(const CiBitVector& other) {
  this->op_xor(other, CiBit::zero);
  return *this;
}

CiBitVector& CiBitVector::operator&=(const CiBit& p_bit) {
  this->op_and(p_bit);
  return *this;
}

CiBitVector& CiBitVector::operator|=(const CiBit& p_bit) {
  this->op_or(p_bit);
  return *this;
}

CiBitVector& CiBitVector::operator^=(const CiBit& p_bit) {
  this->op_xor(p_bit);
  return *this;
}

CiBitVector& CiBitVector::op_not() {
  for (unsigned i = 0; i < size(); ++i) {
    (*this)[i].op_not();
  }
  return *this;
}

#define DEFINE_BITWISE_MEMBER_FUNC_BV(OP_NAME, SAME_OPERANDS_CODE) \
CiBitVector& CiBitVector::OP_NAME(const CiBitVector& other, const CiBit& p_bit) { \
  if (this == &other) { \
    SAME_OPERANDS_CODE; \
  } else { \
    unsigned n = min(size(), other.size()); \
    for (unsigned i = 0; i < n; ++i) { \
      (*this)[i].OP_NAME(other[i]); \
    } \
    for (unsigned i = n; i < size(); ++i) { \
      (*this)[i].OP_NAME(p_bit); \
    } \
  } \
  return *this; \
}

DEFINE_BITWISE_MEMBER_FUNC_BV(op_and  , );
DEFINE_BITWISE_MEMBER_FUNC_BV(op_nand , op_not());
DEFINE_BITWISE_MEMBER_FUNC_BV(op_andny, m_vec.assign(size(), CiBit::zero));
DEFINE_BITWISE_MEMBER_FUNC_BV(op_andyn, m_vec.assign(size(), CiBit::zero));
DEFINE_BITWISE_MEMBER_FUNC_BV(op_or   , );
DEFINE_BITWISE_MEMBER_FUNC_BV(op_nor  , op_not());
DEFINE_BITWISE_MEMBER_FUNC_BV(op_orny , m_vec.assign(size(), CiBit::one));
DEFINE_BITWISE_MEMBER_FUNC_BV(op_oryn , m_vec.assign(size(), CiBit::one));
DEFINE_BITWISE_MEMBER_FUNC_BV(op_xor  , m_vec.assign(size(), CiBit::zero));
DEFINE_BITWISE_MEMBER_FUNC_BV(op_xnor , m_vec.assign(size(), CiBit::one));

#define DEFINE_BITWISE_MEMBER_FUNC_B(OP_NAME) \
CiBitVector& CiBitVector::OP_NAME(const CiBit& p_bit) { \
  for (unsigned i = 0; i < size(); ++i) { \
    (*this)[i].OP_NAME(p_bit); \
  } \
  return *this; \
}

DEFINE_BITWISE_MEMBER_FUNC_B(op_and);
DEFINE_BITWISE_MEMBER_FUNC_B(op_nand);
DEFINE_BITWISE_MEMBER_FUNC_B(op_andny);
DEFINE_BITWISE_MEMBER_FUNC_B(op_andyn);
DEFINE_BITWISE_MEMBER_FUNC_B(op_or);
DEFINE_BITWISE_MEMBER_FUNC_B(op_nor);
DEFINE_BITWISE_MEMBER_FUNC_B(op_orny);
DEFINE_BITWISE_MEMBER_FUNC_B(op_oryn);
DEFINE_BITWISE_MEMBER_FUNC_B(op_xor);
DEFINE_BITWISE_MEMBER_FUNC_B(op_xnor);


CiBitVector& CiBitVector::shl(const int pos, const CiBit& p_bit) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return this->shr(-pos, p_bit);

  if (pos > 0) {
    unsigned ppos = (pos > (int)size()) ? size() : pos;
    m_vec.resize(size() + ppos, p_bit);
    m_vec.erase(m_vec.begin(), m_vec.begin() + ppos);
  }

  return *this;
}

CiBitVector& CiBitVector::shr(const int pos, const CiBit& p_bit) {
  if (size() == 0)
    return *this;

  if (pos < 0)
    return this->shl(-pos, p_bit);

  if (pos > 0) {
    unsigned ppos = (pos > (int)size()) ? size() : pos;
    m_vec.resize(size() - ppos);
    m_vec.insert(m_vec.begin(), ppos, p_bit);
  }

  return *this;
}

CiBitVector& CiBitVector::rol(const int pos) {
  if (size() > 0 and pos != 0) {
    unsigned ppos = (pos > 0) ? (pos % size()) : (size() - (-pos % size()));

    auto bits_cpy = m_vec;
    m_vec = vector<CiBit>(bits_cpy.begin()+ppos, bits_cpy.end());
    m_vec.insert(m_vec.end(), bits_cpy.begin(), bits_cpy.begin()+ppos);
  }

  return *this;
}

CiBitVector& CiBitVector::ror(const int pos) {
  return rol(-pos);
}

CiBitVector& CiBitVector::operator<<= (const int pos) {
  return shl(pos, CiBit::zero);
}

CiBitVector& CiBitVector::operator>>= (const int pos) {
  return shr(pos, CiBit::zero);
}


int CiBitVector::idx_rel_to_abs(int idx) const {
  if (idx < 0)
    idx += size();
  return idx;
}

unsigned CiBitVector::idx_clip(int idx) const {
  if (idx < 0)
    idx = 0;
  else if (idx > (int)size())
    idx = size();
  return idx;
}

CiBitVector cingulata::operator~(CiBitVector lhs) {
  return lhs.op_not();
}

CiBitVector cingulata::operator^(CiBitVector lhs, const CiBitVector& rhs) {
  return lhs ^= rhs;
}

CiBitVector cingulata::operator&(CiBitVector lhs, const CiBitVector& rhs) {
  return lhs &= rhs;
}

CiBitVector cingulata::operator|(CiBitVector lhs, const CiBitVector& rhs) {
  return lhs |= rhs;
}

CiBitVector cingulata::shl(CiBitVector lhs, const int pos, const CiBit& p_bit) {
  return lhs.shl(pos, p_bit);
}

CiBitVector cingulata::shr(CiBitVector lhs, const int pos, const CiBit& p_bit) {
  return lhs.shr(pos, p_bit);
}

CiBitVector cingulata::rol(CiBitVector lhs, const int pos) {
  return lhs.rol(pos);
}

CiBitVector cingulata::ror(CiBitVector lhs, const int pos) {
  return lhs.ror(pos);
}

CiBitVector cingulata::operator<< (CiBitVector lhs, const int pos) {
  return lhs <<= pos;
}

CiBitVector cingulata::operator>> (CiBitVector lhs, const int pos) {
  return lhs >>= pos;
}

istream& cingulata::operator>>(istream& inp, CiBitVector& val) {
  val.read();
  return inp;
}

ostream& cingulata::operator<<(ostream& out, CiBitVector& val) {
  val.write();
  return out;
}

