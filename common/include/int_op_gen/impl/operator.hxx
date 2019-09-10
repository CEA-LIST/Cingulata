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

#ifndef COMMON_OPER
#define COMMON_OPER

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>

#include <vector>

namespace cingulata {
namespace int_ops {

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
 * @brief      Binary operator base class
 */
class AdderOper {
public:
  using signature = CiBitVector(const CiBitVector &, const CiBitVector &,
                                const CiBit &);

  CiBitVector operator()(const CiBitVector &, const CiBitVector &,
                         const CiBit & = CiBit::zero) const;

private:
  virtual CiBitVector oper(const CiBitVector &, const CiBitVector &,
                           const CiBit &) const = 0;
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
class SortOper {
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

} // namespace int_ops
} // namespace cingulata
#endif
