#include <int_op_gen/interface.hxx>

using namespace std;
using namespace cingulata;

/*Sergiu needs to add a comment here
*  His genius can't be understood by common mortals
*/
IIntOpGen::IIntOpGen() :
m_sort{bind(&IIntOpGen::lower,this,placeholders::_1,placeholders::_2),
	   bind(&IIntOpGen::equal,this,placeholders::_1,placeholders::_2),
	   bind(&IIntOpGen::add,this,placeholders::_1,placeholders::_2)}
{}

CiBitVector IIntOpGen::sub(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return add(lhs, neg(rhs));
}

CiBitVector IIntOpGen::square(const CiBitVector& rhs) const {
  return mul(rhs, rhs);
}

CiBit IIntOpGen::not_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !equal(lhs,rhs);
}

CiBit IIntOpGen::greater(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return lower(rhs, lhs);
}

CiBit IIntOpGen::lower_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !greater(lhs, rhs);
}

CiBit IIntOpGen::greater_equal(const CiBitVector& lhs, const CiBitVector& rhs) const {
  return !lower(lhs, rhs);
}

CiBitVector IIntOpGen::mux(const CiBit &cond, const CiBitVector &a,
                           const CiBitVector &b) const {
  return mux(CiBitVector{1, cond}, {a, b});
}

CiBitVector IIntOpGen::mux(const CiBitVector &cond, const vector<CiBitVector> &inps) const {
  return m_mux(cond, inps);
}

std::vector<CiBitVector> IIntOpGen::sort (const std::vector<CiBitVector> &v_cbv,
	                                      const std::vector<CiBitVector> &i_cbv,
	                                      const bool reverse) const{
  return m_sort(v_cbv, i_cbv, reverse);
}
