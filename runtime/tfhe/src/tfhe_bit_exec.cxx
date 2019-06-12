/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#include <tfhe_bit_exec.hxx>

#include <tfhe.h>
#include <tfhe_io.h>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cingulata;

/**
 * @brief      TFHE context class (storage for paramters and secret/public keys
 *             objects)
 */
class TfheBitExec::Context {
public:
  Context(const string &p_filename, const KeyType p_keytype) {
    ifstream file(p_filename.c_str());
    if (not file.is_open()) {
      cerr << "ERROR TfheBitExec::Context: Cannot open key file '" << p_filename
           << "'" << endl;
      abort();
    }

    if (p_keytype == Secret) {
      m_sk = new_tfheGateBootstrappingSecretKeySet_fromStream(file);
      m_pk = const_cast<TFheGateBootstrappingCloudKeySet *>(
          &(m_sk->cloud)); // no other easy way to avoid const cast
    } else {
      m_sk = nullptr;
      m_pk = new_tfheGateBootstrappingCloudKeySet_fromStream(file);
    }
    file.close();

    m_params = m_pk->params;
  }

  ~Context() {
    if (m_sk != nullptr) {
      delete_gate_bootstrapping_secret_keyset(m_sk);
    } else {
      delete_gate_bootstrapping_cloud_keyset(m_pk);
    }
  }

  const TFheGateBootstrappingSecretKeySet *const sk() const {
    assert(m_sk != nullptr && "secret key was not set");
    return m_sk;
  }

  const TFheGateBootstrappingCloudKeySet *const pk() const { return m_pk; }

  const TFheGateBootstrappingParameterSet *const params() const {
    return m_params;
  }

private:
  TFheGateBootstrappingSecretKeySet *m_sk = nullptr;
  TFheGateBootstrappingCloudKeySet *m_pk = nullptr;
  const TFheGateBootstrappingParameterSet *m_params = nullptr;
};

/**
 * @brief      TFHE allocator class -- creates and destroys TFHE LWE samples
 */
class TfheBitExec::Alloc {
  const TFheGateBootstrappingParameterSet *const m_params = nullptr;

public:
  Alloc(const TFheGateBootstrappingParameterSet *const p_params)
      : m_params(p_params) {}

  void *new_obj() const { return new_gate_bootstrapping_ciphertext(m_params); }

  void del_obj(void *obj_ptr) const {
    delete_gate_bootstrapping_ciphertext(static_cast<LweSample *>(obj_ptr));
  }
};

TfheBitExec::TfheBitExec(const string &p_filename, KeyType p_keytype)
    : context(new Context(p_filename, p_keytype)),
      alloc(new Alloc(context->params())), mm(new ObjMan(*alloc)) {}

TfheBitExec::~TfheBitExec() {
  delete mm;
  delete alloc;
  delete context;
}

ObjHandle TfheBitExec::encode(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = mm->new_handle();
  bootsCONSTANT(hdl.get(), pt_val, context->pk());
  return hdl;
}

ObjHandle TfheBitExec::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<LweSample> hdl = mm->new_handle();
  bootsSymEncrypt(hdl.get(), pt_val % 2, context->sk());
  return hdl;
}

bit_plain_t TfheBitExec::decrypt(const ObjHandle &in) {
  return (bit_plain_t)bootsSymDecrypt(in.get<LweSample>(), context->sk());
}

ObjHandle TfheBitExec::read(const string &name) {
  ifstream file(name);
  if (not file.is_open()) {
    cerr << "ERROR TfheBitExec::read: Cannot open file '" << name << "'"
         << endl;
    abort();
  }

  ObjHandleT<LweSample> hdl = mm->new_handle();
  import_gate_bootstrapping_ciphertext_fromStream(file, hdl.get(),
                                                  context->params());
  file.close();

  return hdl;
}

void TfheBitExec::write(const ObjHandle &in, const string &name) {
  ofstream file(name);
  if (not file.is_open()) {
    cerr << "ERROR TfheBitExec::write : cannot open file '" << name << "'"
         << endl;
    abort();
  }
  export_gate_bootstrapping_ciphertext_toStream(file, in.get<LweSample>(),
                                                context->params());
}

ObjHandle TfheBitExec::op_not(const ObjHandle &in) {
  ObjHandleT<LweSample> hdl = mm->new_handle();
  bootsNOT(hdl.get(), in.get<LweSample>(), context->pk());
  return hdl;
}

#define TFHE_EXEC_OPER(OPER, TFHE_FNC)                                         \
  ObjHandle TfheBitExec::OPER(const ObjHandle &in1, const ObjHandle &in2) {    \
    ObjHandleT<LweSample> hdl = mm->new_handle();                              \
    TFHE_FNC(hdl.get(), in1.get<LweSample>(), in2.get<LweSample>(),            \
             context->pk());                                                   \
    return hdl;                                                                \
  }

TFHE_EXEC_OPER(op_and, bootsAND);
TFHE_EXEC_OPER(op_xor, bootsXOR);
TFHE_EXEC_OPER(op_nand, bootsNAND);
TFHE_EXEC_OPER(op_andyn, bootsANDYN);
TFHE_EXEC_OPER(op_andny, bootsANDNY);
TFHE_EXEC_OPER(op_or, bootsOR);
TFHE_EXEC_OPER(op_nor, bootsNOR);
TFHE_EXEC_OPER(op_oryn, bootsORYN);
TFHE_EXEC_OPER(op_orny, bootsORNY);
TFHE_EXEC_OPER(op_xnor, bootsXNOR);

ObjHandle TfheBitExec::op_mux(const ObjHandle &cond, const ObjHandle &in1,
                              const ObjHandle &in2) {
  ObjHandleT<LweSample> hdl = mm->new_handle();
  bootsMUX(hdl.get(), cond.get<LweSample>(), in1.get<LweSample>(),
           in2.get<LweSample>(), context->pk());
  return hdl;
}
