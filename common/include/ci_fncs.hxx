#ifndef CI_FNCS
#define CI_FNCS

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <ci_int.hxx>

namespace cingulata {
  CiInt select(const CiBit &cond, const CiInt &a, const CiInt &b);
}

#endif // CI_FNCS
