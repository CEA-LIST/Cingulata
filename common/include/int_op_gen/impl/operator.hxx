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
class UnaryOper {
public:
  using signature = CiBitVector(const CiBitVector &);

  CiBitVector operator()(const CiBitVector &) const;

private:
  virtual CiBitVector oper(const CiBitVector &) const = 0;
};

/**
 * @brief      Binary operator base class
 */
class BinaryOper {
public:
  using signature = CiBitVector(const CiBitVector &, const CiBitVector &);

  CiBitVector operator()(const CiBitVector &, const CiBitVector &) const;

private:
  virtual CiBitVector oper(const CiBitVector &, const CiBitVector &) const = 0;
};

/**
 * @brief      N-ary operator base class
 */
class NaryOper {
public:
  using signature = CiBitVector(const std::vector<CiBitVector> &);

  CiBitVector operator()(const std::vector<CiBitVector> &) const;

private:
  virtual CiBitVector oper(const std::vector<CiBitVector> &) const = 0;
};

/**
 * @brief      Comparison operator base class
 */
class CompOper {
public:
  using signature = CiBit(const CiBitVector &, const CiBitVector &);

  CiBit operator()(const CiBitVector &, const CiBitVector &) const;

private:
  virtual CiBit oper(const CiBitVector &, const CiBitVector &) const = 0;
};

/**
 * @brief      Multiplexer operator base class
 */
class MuxOper {
public:
  using signature = CiBitVector(const CiBitVector &,
                                const std::vector<const CiBitVector &> &);

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
  CiBitVector operator()(const CiBitVector &cond,
                         const std::vector<CiBitVector> &inps) const;

private:
  virtual CiBitVector oper(const CiBitVector &cond,
                           const std::vector<CiBitVector> &inps) const = 0;
};

/**
 * @brief      Sort operator base class
 */
class SortOper
{
public:
  using signature = std::vector<CiBitVector>(const std::vector<CiBitVector> &,
                                             const std::vector<CiBitVector> &,
                                             const bool);

  /**
   * @brief      Sort the element of vector @c v_cbv and return the sorted
   *             vector. The result is given in decreasing order if @c reverse
   *             is set with 1.
   *
   * @details    All the elements of @v_cbv must have the same bit-size. The
   *             bit-size of result is the same as bit-size of an element of @c
   *             i_cbv.
   *
   * @param[in]  v_vbv    Vector of CiBitVector to be sort
   * @param[in]  reverse  0 if ascending order, 1 else
   *
   * @return     sorted vector of input vector
   */
  std::vector<CiBitVector> operator()(const std::vector<CiBitVector> &v_cbv,
                                      const bool reverse) const;

  /**
   * @brief      Sort the element by @c v_cbv and return the corresponding
   *             sorted array of @c i_cbv. The result is given in decreasing
   *             order if @c reverse is set with 1.
   *
   * @details    The number of elements in @c v_cbv must be equal to the number
   *             of elements of @c i_cbv. All the elements of @c v_cbv and @c
   *             i_cbv must have the same bit-size. The bit-size of result is
   *             the same as bit-size of an element of @c i_cbv.
   *
   * @param[in]  v_vbv    Vector of CiBitVector (metric to sort)
   * @param[in]  i_vbv    Vector of CiBitVector (metric to retrieve)
   * @param[in]  reverse  0 if ascending order, 1 else
   *
   * @return     sorted vector of input vector
   */
  std::vector<CiBitVector> operator()(const std::vector<CiBitVector> &v_cbv,
                                      const std::vector<CiBitVector> &i_cbv,
                                      const bool reverse) const;

private:
  virtual std::vector<CiBitVector> oper(const std::vector<CiBitVector> &v_cbv,
                                        const std::vector<CiBitVector> &i_cbv,
                                        const bool reverse) const = 0;
};

}
}
#endif
