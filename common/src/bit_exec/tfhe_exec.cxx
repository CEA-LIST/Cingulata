#include <bit_exec/tfhe_exec.hxx>

#include <tfhe_io.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cingulata;

TfheExecutor::TfheExecutor(
    const TFheGateBootstrappingCloudKeySet *const p_pks,
    const TFheGateBootstrappingSecretKeySet *const p_sks)
    : public_keyset(p_pks), secret_keyset(p_sks) {}

ObjHandle TfheExecutor::encode(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = new_handle();
  lweNoiselessTrivial(hdl.get(), pt_val, public_keyset->params->in_out_params);
  return hdl;
}

ObjHandle TfheExecutor::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsSymEncrypt(hdl.get(), pt_val%2, secret_keyset);
  return hdl;
}

IBitExec::bit_plain_t TfheExecutor::decrypt(const ObjHandle& in) {
  return (bit_plain_t)bootsSymDecrypt(in.get<LweSample>(), secret_keyset);
}

ObjHandle TfheExecutor::read(const std::string& name) {
  ifstream file(name);
  if (not file.is_open()) {
    return ObjHandle();
  }

  ObjHandleT<LweSample> hdl = new_handle();
  import_lweSample_fromStream(file, hdl.get(), public_keyset->params->in_out_params);
  return hdl;
}

void TfheExecutor::write(const ObjHandle& in, const std::string& name) {
  ofstream file(name);
  if (not file.is_open()) {
    cerr << "TfheExecutor::write : cannot open file '" << name << "'" << endl;
    return;
  }
  export_lweSample_toStream(file, in.get<LweSample>(), public_keyset->params->in_out_params);
}

ObjHandle TfheExecutor::op_not(const ObjHandle& in) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsNOT(hdl.get(), in.get<LweSample>(), public_keyset);
  return hdl;
}

#define TFHE_EXEC_OPER(OPER, TFHE_FNC)                                         \
  ObjHandle TfheExecutor::OPER(const ObjHandle &in1, const ObjHandle &in2) {   \
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

ObjHandle TfheExecutor::op_mux(const ObjHandle &cond, const ObjHandle &in1,
                               const ObjHandle &in2) {
  ObjHandleT<LweSample> hdl = new_handle();
  bootsMUX(hdl.get(), cond.get<LweSample>(), in1.get<LweSample>(),
           in2.get<LweSample>(), public_keyset);
  return hdl;
}

void* TfheExecutor::new_obj() {
  return new_LweSample(public_keyset->params->in_out_params);
}

void TfheExecutor::del_obj(void * obj_ptr) {
  delete_LweSample((LweSample*)obj_ptr);
}


