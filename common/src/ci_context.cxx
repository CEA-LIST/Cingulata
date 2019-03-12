#include <ci_context.hxx>
#include <bit_exec/tracker.hxx>
#include <int_op_gen/mult_depth.hxx>

using namespace cingulata;


IBitExec* CiContext::m_bit_exec{new BitTracker()};
IIntOpGen* CiContext::m_int_op_gen{new IntOpGenDepth()};
