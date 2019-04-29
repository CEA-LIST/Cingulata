#ifndef CI_FNCS
#define CI_FNCS

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_int.hxx>

namespace cingulata {
  /**
   * @brief      Oblivious select one of two integers.
   * @details    This function is an equivalent of the ternary operator @c
   *             cond?a:b
   *
   * @param[in]  cond  The condition
   * @param[in]  a     Result when @c condition==1
   * @param[in]  b     Result when @c condition==0
   *
   * @return     obliviously selected integer
   */
  CiInt select(const CiBit &cond, const CiInt &a, const CiInt &b);

  /**
   * @brief      Sums-up a list of integers.
   * @details    Resulting @c CiInt object is minimal in bit-size.
   *
   * @param[in]  elems  The elements to add
   *
   * @return     Sum in input elements
   */
  CiInt sum(const std::vector<CiInt> &elems);

  /**
   * @brief      Sums-up a list of bits.
   * @details    Resulting @c CiInt object is minimal in bit-size.
   *
   * @param[in]  elems  The elements to add
   *
   * @return     Sum in input elements
   */
  CiInt sum(const std::vector<CiBit> &elems);

  /**
   * @brief      Oblivious sort a vector of @c CiInt objects @c vals
   *
   * @param[in]  vals    The elements to sort
   * @param[in]  reverse  The sort order
   *
   * @return     Sorted vector of integers
   */
  std::vector<CiInt> sort(const std::vector<CiInt> &vals,
                          const bool reverse = false);

  /**
   * @brief      Oblivious sort a vector of @c CiInt objects @c elems according
   *             to values in vector @c vals.
   * @details    Output
   *
   * @param[in]  vals    Values to use for comparison
   * @param[in]  elems    The elements to sort
   * @param[in]  reverse  The sort order
   *
   * @return     Sorted vector of integers
   */
  std::vector<CiInt> sort(const std::vector<CiInt> &vals,
                          const std::vector<CiInt> &elems,
                          const bool reverse = false);

  /**
   * @brief      Oblivious argument sort a vector of @c CiInt objects @c vals,
   *             i.e. returns indices (encrypted) which sort input vector @c
   *             vals.
   *
   * @param[in]  vals     The elements to argument sort
   * @param[in]  reverse  The sort order
   *
   * @return     Sorted vector of integers
   */
  std::vector<CiInt> argsort(const std::vector<CiInt> &vals,
                             const bool reverse = false);

  } // namespace cingulata

#endif // CI_FNCS
