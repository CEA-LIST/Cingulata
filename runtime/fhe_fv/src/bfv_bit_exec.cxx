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

#include <bfv_bit_exec.hxx>
#include <fv.hxx>

using namespace std;
using namespace cingulata;

class BfvBitExec::Context {
public:
  Context(const string &p_param, const string &p_key_prefix,
          const KeyType p_keytype) {
    /* read BFV scheme parameters */
    FheParams::readXml(p_param.c_str());

    if (p_keytype == Secret) {
      m_sk = new KeysAll();
      m_sk->readKeys(p_key_prefix);
      m_pk = static_cast<KeysShare *>(m_sk);
    } else {
      m_sk = nullptr;
      m_pk = new KeysShare();
      m_pk->readKeys(p_key_prefix);
    }
  }

  ~Context() {
    if (m_sk != nullptr) {
      delete m_sk;
    } else {
      delete m_pk;
    }
  }

  const PolyRing& sk() const {
    assert(m_sk != nullptr && "secret key was not set");
    return *(m_sk->SecretKey);
  }

  const CipherText& pk() const { return *(m_pk->PublicKey); }

  const CipherText& evk() const { return *(m_pk->EvalKey); }

private:
  KeysAll *m_sk = nullptr;
  KeysShare *m_pk = nullptr;
};

BfvBitExec::BfvBitExec(const string &p_param, const string &p_key_prefix,
                       const KeyType p_keytype)
    : context(new Context(p_param, p_key_prefix, p_keytype)),
      mm(new ObjMan()) {}

ObjHandle BfvBitExec::encode(const bit_plain_t pt_val) {
  ObjHandleT<CipherText> hdl = mm->new_handle();
  EncDec::Encrypt(*hdl.get(), pt_val);
  return hdl;
}

ObjHandle BfvBitExec::encrypt(const bit_plain_t pt_val) {
  ObjHandleT<CipherText> hdl = mm->new_handle();
  EncDec::Encrypt(*hdl.get(), pt_val, context->pk());
  return hdl;
}

bit_plain_t BfvBitExec::decrypt(const ObjHandle& in) {
  return EncDec::Decrypt(*in.get<CipherText>(), context->sk());
}

ObjHandle BfvBitExec::read(const std::string& name) {
  ObjHandleT<CipherText> hdl = mm->new_handle();
  hdl->read(name);
  return hdl;
}

void BfvBitExec::write(const ObjHandle& in, const std::string& name) {
  in.get<CipherText>()->write(name);
}

ObjHandle BfvBitExec::op_and(const ObjHandle& in1, const ObjHandle& in2) {
  ObjHandleT<CipherText> hdl = mm->new_handle();
  CipherText::copy(*hdl.get(), *in1.get<CipherText>());
  CipherText::multiply(*hdl.get(), *in2.get<CipherText>(), context->evk());
  return hdl;
}

ObjHandle BfvBitExec::op_xor(const ObjHandle& in1, const ObjHandle& in2) {
  ObjHandleT<CipherText> hdl = mm->new_handle();
  CipherText::copy(*hdl.get(), *in1.get<CipherText>());
  CipherText::add(*hdl.get(), *in2.get<CipherText>());
  return hdl;
}
