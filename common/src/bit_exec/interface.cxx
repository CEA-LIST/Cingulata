#include <bit_exec/interface.hxx>
#include <bit_exec/obj_handle.hxx>

using namespace cingulata;

ObjHandle IBitExec::op_nand(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_not(op_and(lhs, rhs));
}

ObjHandle IBitExec::op_andyn(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_and(lhs, op_not(rhs));
}

ObjHandle IBitExec::op_andny(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_and(op_not(lhs), rhs);
}

ObjHandle IBitExec::op_or(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_xor(rhs, op_xor(lhs, op_and(lhs, rhs)));
}

ObjHandle IBitExec::op_nor(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_not(op_or(lhs, rhs));
}

ObjHandle IBitExec::op_oryn(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_or(lhs, op_not(rhs));
}

ObjHandle IBitExec::op_orny(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_or(op_not(lhs), rhs);
}

ObjHandle IBitExec::op_xnor(const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_not(op_xor(lhs, rhs));
}

ObjHandle IBitExec::op_mux(const ObjHandle& c, const ObjHandle& lhs, const ObjHandle& rhs) {
  return op_xor(op_and(c, op_xor(lhs, rhs)), rhs);
}

ObjHandle IBitExec::new_handle() {
  return ObjHandle(new_obj(), [this](void* p) { del_obj(p); });
}
