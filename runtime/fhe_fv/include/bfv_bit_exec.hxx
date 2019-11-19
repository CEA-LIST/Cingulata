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

#ifndef BFV_BIT_EXECUTOR
#define BFV_BIT_EXECUTOR

// #define USE_OBJ_POOL // Object Pool is not thread safe

#include <fv.hxx>

#include <bit_exec/interface_she.hxx>
#include <bit_exec/obj_man/allocator.hxx>

#ifdef USE_OBJ_POOL
#include <bit_exec/obj_man/pool.hxx>
#else
#include <bit_exec/obj_man/basic.hxx>
#endif

namespace cingulata {

/**
 * @brief      Bit executor implemenation for TFHE library.
 */
class BfvBitExec : public IBitExecSHE {
public:
  enum KeyType { Secret, Public };
  class Context;

  BfvBitExec(Context *p_context);
  BfvBitExec(const std::string &p_param, const std::string &p_key_filename,
             const KeyType p_keytype);
  ~BfvBitExec();

  /* clang-format off */

  ObjHandle   encode      (const bit_plain_t pt_val)                      override;
  ObjHandle   encrypt     (const bit_plain_t pt_val)                      override;
  bit_plain_t decrypt     (const ObjHandle& in)                           override;
  ObjHandle   read        (const std::string& name)                       override {
    return read(name, binary_input);
  }
  void        write       (const ObjHandle& in, const std::string& name)  override {
    write(in, name, binary_output);
  }

  ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    override;

  /* ring-lwe specific functions */
  ObjHandle   read        (const std::string& name, bool binary);
  void        write       (const ObjHandle& in, const std::string& name, bool binary);
  ObjHandle   encode      (const std::vector<bit_plain_t>& vals);
  ObjHandle   encrypt     (const std::vector<bit_plain_t>& vals);
  void        decrypt     (std::vector<bit_plain_t>& vals, const ObjHandle& in);
  double      noise       (const ObjHandle& in);

  /* clang-format on */

  bool binary_input = true;
  bool binary_output = true;

protected:
  const Context *context;

#ifdef USE_OBJ_POOL
  using ObjMan = obj_man::Pool<obj_man::Allocator<CipherText>>;
#else
  using ObjMan = obj_man::Basic<obj_man::Allocator<CipherText>>;
#endif

  ObjMan *mm;
};

class BfvBitExec::Context {
public:
  Context(const std::string &p_param, const std::string &p_key_filename,
          const KeyType p_keytype);

  ~Context();

  const PolyRing &sk() const;
  const CipherText &pk() const;
  const CipherText &evk() const;

private:
  SecretKey *m_sk = nullptr;
  PublicKey *m_pk = nullptr;
};

}; // namespace cingulata

#endif
