#include <tfhe_bit_exec.hxx>

#include <tfhe_io.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cingulata;

TfheBitExec::TfheBitExec(
    const TFheGateBootstrappingCloudKeySet *const p_pks,
    const TFheGateBootstrappingSecretKeySet *const p_sks)
    : public_keyset(p_pks), secret_keyset(p_sks) {}

ObjHandle TfheBitExec::encode(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = new_handle();
  lweNoiselessTrivial(hdl.get(), pt_val, public_keyset->params->in_out_params);
  return hdl;
}

ObjHandle TfheBitExec::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsSymEncrypt(hdl.get(), pt_val%2, secret_keyset);
  return hdl;
}

IBitExec::bit_plain_t TfheBitExec::decrypt(const ObjHandle& in) {
  return (bit_plain_t)bootsSymDecrypt(in.get<LweSample>(), secret_keyset);
}

ObjHandle TfheBitExec::read(const std::string& name) {
  ifstream file(name);
  if (not file.is_open()) {
    return ObjHandle();
  }

  ObjHandleT<LweSample> hdl = new_handle();
  import_lweSample_fromStream(file, hdl.get(), public_keyset->params->in_out_params);
  return hdl;
}

void TfheBitExec::write(const ObjHandle& in, const std::string& name) {
  ofstream file(name);
  if (not file.is_open()) {
    cerr << "TfheBitExec::write : cannot open file '" << name << "'" << endl;
    return;
  }
  export_lweSample_toStream(file, in.get<LweSample>(), public_keyset->params->in_out_params);
}

ObjHandle TfheBitExec::op_not(const ObjHandle& in) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsNOT(hdl.get(), in.get<LweSample>(), public_keyset);
  return hdl;
}

#define TFHE_EXEC_OPER(OPER, TFHE_FNC)                                         \
  ObjHandle TfheBitExec::OPER(const ObjHandle &in1, const ObjHandle &in2) {   \
    ObjHandleT<LweSample> hdl = new_handle();                                  \
    TFHE_FNC(hdl.get(), in1.get<LweSample>(), in2.get<LweSample>(),            \
             public_keyset);                                                   \
    return hdl;                                                                \
  }

TFHE_EXEC_OPER(op_and,    bootsAND);
TFHE_EXEC_OPER(op_xor,    bootsXOR);
TFHE_EXEC_OPER(op_nand,   bootsNAND);
TFHE_EXEC_OPER(op_andyn,  bootsANDYN);
TFHE_EXEC_OPER(op_andny,  bootsANDNY);
TFHE_EXEC_OPER(op_or,     bootsOR);
TFHE_EXEC_OPER(op_nor,    bootsNOR);
TFHE_EXEC_OPER(op_oryn,   bootsORYN);
TFHE_EXEC_OPER(op_orny,   bootsORNY);
TFHE_EXEC_OPER(op_xnor,   bootsXNOR);

ObjHandle TfheBitExec::op_mux(const ObjHandle &cond, const ObjHandle &in1,
                               const ObjHandle &in2) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsMUX(hdl.get(), cond.get<LweSample>(), in1.get<LweSample>(),
           in2.get<LweSample>(), public_keyset);
  return hdl;
}

void* TfheBitExec::new_obj() {
  return new_LweSample(public_keyset->params->in_out_params);
}

void TfheBitExec::del_obj(void * obj_ptr) {
  delete_LweSample((LweSample*)obj_ptr);
}


