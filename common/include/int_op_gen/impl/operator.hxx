#ifndef COMMON_OPER
#define COMMON_OPER

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>

#include <vector>

namespace cingulata
{
namespace int_ops
{

/**
 * @brief      Unary operator base class
 */
class UnaryOper
{
public:
  using signature = CiBitVector (const CiBitVector& rhs);

  CiBitVector operator()(const CiBitVector& rhs) const;

private:
  virtual CiBitVector oper(const CiBitVector&) const = 0;
};

/**
 * @brief      Binary operator base class
 */
class BinaryOper
{
public:
  using signature = CiBitVector (const CiBitVector&, const CiBitVector&);

  CiBitVector operator()(const CiBitVector& lhs, const CiBitVector& rhs) const;

private:
  virtual CiBitVector oper(const CiBitVector&, const CiBitVector&) const = 0;
};

/**
 * @brief      N-ary operator base class
 */
class NaryOper
{
public:
  using signature = CiBitVector (const std::vector<CiBitVector>&);

  CiBitVector operator()(const std::vector<CiBitVector>&) const;

private:
  virtual CiBitVector oper(const std::vector<CiBitVector>&) const = 0;
};

/**
 * @brief      Comparison operator base class
 */
class CompOper
{
public:
  using signature = CiBit (const CiBitVector&, const CiBitVector&);

  CiBit operator()(const CiBitVector& lhs, const CiBitVector& rhs) const;

private:
  virtual CiBit oper(const CiBitVector&, const CiBitVector&) const = 0;
};

/**
 * @brief      Multiplexer operator base class
 */
class MuxOper
{
public:
  using signature = CiBitVector (const CiBitVector&, const std::vector<const CiBitVector&>& inps);

  /**
   * @brief      Selects the k-th element from vector @c inps where the
   *             binary form of k is given by @c cond.
   * @details    The number of elements in @c inps must be equal to @c
   *             2^cond.size(). All the elements of @c inps must have the
   *             same bit-size. The bit-size of result is the same as
   *             bit-size of an @c inps element.
   *
   * @param[in]  cond  Selection condition
   * @param[in]  inps  Vector of input words
   *
   * @return     selected word
   */
  CiBitVector operator()(const CiBitVector& cond, const std::vector<CiBitVector>& inps) const;

private:
  virtual CiBitVector oper(const CiBitVector& cond, const std::vector<CiBitVector>& inps) const = 0;
};

}
}
#endif
