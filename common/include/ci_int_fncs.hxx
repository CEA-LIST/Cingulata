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
   * @brief      Sums-up many integers.
   * @details    Resulting @c CiInt object is minimal in bit-size.
   *
   * @param[in]  elems  The elements to add
   *
   * @return     Sum in input elements
   */
  CiInt sum(const std::vector<CiInt> &elems);
}

#endif // CI_FNCS
