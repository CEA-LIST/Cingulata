#include <bit_exec/interface.hxx>
#include <bit_exec/obj_handle.hxx>

using namespace cingulata;

ObjHandle IBitExec::op_not(const ObjHandle& in1) {
  return op_xor(in1, encode(1));
}

ObjHandle IBitExec::op_nand(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_and(in1, in2));
}

ObjHandle IBitExec::op_andyn(const ObjHandle& in1, const ObjHandle& in2) {
  return op_and(in1, op_not(in2));
}

ObjHandle IBitExec::op_andny(const ObjHandle& in1, const ObjHandle& in2) {
  return op_and(op_not(in1), in2);
}

ObjHandle IBitExec::op_or(const ObjHandle& in1, const ObjHandle& in2) {
  return op_xor(in2, op_xor(in1, op_and(in1, in2)));
}

ObjHandle IBitExec::op_nor(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_or(in1, in2));
}

ObjHandle IBitExec::op_oryn(const ObjHandle& in1, const ObjHandle& in2) {
  return op_or(in1, op_not(in2));
}

ObjHandle IBitExec::op_orny(const ObjHandle& in1, const ObjHandle& in2) {
  return op_or(op_not(in1), in2);
}

ObjHandle IBitExec::op_xnor(const ObjHandle& in1, const ObjHandle& in2) {
  return op_not(op_xor(in1, in2));
}

ObjHandle IBitExec::op_mux(const ObjHandle& c, const ObjHandle& in1, const ObjHandle& in2) {
  return op_xor(op_and(c, op_xor(in1, in2)), in2);
}

ObjHandle IBitExec::new_handle() {
  return ObjHandle(new_obj(), [this](void* p) { del_obj(p); });
}
