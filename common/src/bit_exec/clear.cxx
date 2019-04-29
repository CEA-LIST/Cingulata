#include <bit_exec/clear.hxx>

using namespace std;
using namespace cingulata;

ObjHandle BitExecClear::encode(const bit_plain_t pt_val) {
  ObjHandleT<bit_plain_t> out = new_handle();
  *out = pt_val;
  return out;
}

ObjHandle BitExecClear::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<bit_plain_t> out = new_handle();
  *out = pt_val;
  return out;
}

bit_plain_t BitExecClear::decrypt(const ObjHandle &in) {
  return *(in.get<bit_plain_t>());
}

ObjHandle BitExecClear::read(const std::string &name) { return ObjHandle(); }

void BitExecClear::write(const ObjHandle &in, const std::string &name) {}

ObjHandle BitExecClear::op_and(const ObjHandle &in1, const ObjHandle &in2) {
  ObjHandleT<bit_plain_t> out = new_handle();
  *out = *in1.get<bit_plain_t>() & *in2.get<bit_plain_t>();
  return out;
}

ObjHandle BitExecClear::op_xor(const ObjHandle &in1, const ObjHandle &in2) {
  ObjHandleT<bit_plain_t> out = new_handle();
  *out = *in1.get<bit_plain_t>() ^ *in2.get<bit_plain_t>();
  return out;
}

void *BitExecClear::new_obj() { return new bit_plain_t; }

void BitExecClear::del_obj(void *obj_ptr) {
  delete (bit_plain_t *)obj_ptr;
}
