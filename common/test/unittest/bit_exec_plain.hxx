#ifndef BIT_EXEC_PLAIN
#define BIT_EXEC_PLAIN

#include <bit_exec/interface.hxx>

using namespace std;
using namespace cingulata;

class BitExecFake : public IBitExec {
public:
  ObjHandle encode(const bit_plain_t pt_val) override {
    ObjHandleT<bit_plain_t> out = new_handle();
    *out = pt_val;
    return out;
  }

  ObjHandle encrypt(const bit_plain_t pt_val) override {
    ObjHandleT<bit_plain_t> out = new_handle();
    *out = pt_val;
    return out;
  }

  bit_plain_t decrypt(const ObjHandle& in1) override {
    return *static_pointer_cast<bit_plain_t>(in1);
  }

  ObjHandle read(const std::string& name) override {}

  void write(const ObjHandle& in1, const std::string& name) override {}

  ObjHandle op_and(const ObjHandle& in1, const ObjHandle& in2) override {
    ObjHandleT<bit_plain_t> out = new_handle();
    *out = *static_pointer_cast<bit_plain_t>(in1) & *static_pointer_cast<bit_plain_t>(in2);
    return out;
  }

  ObjHandle op_xor(const ObjHandle& in1, const ObjHandle& in2) override {
    ObjHandleT<bit_plain_t> out = new_handle();
    *out = *static_pointer_cast<bit_plain_t>(in1) ^ *static_pointer_cast<bit_plain_t>(in2);
    return out;
  }

protected:
  void* new_obj() override {
    return new bit_plain_t;
  }

  void del_obj(void * obj_ptr) override {
    delete (bit_plain_t*)obj_ptr;
  }

};


#endif
