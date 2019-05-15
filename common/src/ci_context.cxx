#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <int_op_gen/mult_depth.hxx>

using namespace cingulata;

std::shared_ptr<IBitExec> CiContext::m_bit_exec{new BitTracker()};
std::shared_ptr<IIntOpGen> CiContext::m_int_op_gen{new IntOpGenDepth()};
