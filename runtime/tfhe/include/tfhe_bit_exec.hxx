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

#ifndef TFHE_EXECUTOR
#define TFHE_EXECUTOR

#define USE_OBJ_POOL

#include <bit_exec/interface.hxx>
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
class TfheBitExec : public IBitExec {
public:
  enum KeyType { Secret, Public };

  TfheBitExec(const std::string &p_filename, const KeyType p_keytype);
  ~TfheBitExec();

  /* clang-format off */
  ObjHandle   encode      (const bit_plain_t pt_val)                      override;
  ObjHandle   encrypt     (const bit_plain_t pt_val)                      override;
  bit_plain_t decrypt     (const ObjHandle& in1)                          override;
  ObjHandle   read        (const std::string& name)                       override;
  void        write       (const ObjHandle& in1, const std::string& name) override;

  ObjHandle   op_not      (const ObjHandle& in1)                          override;
  ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    override;

  ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2)    override;

  ObjHandle   op_mux      (const ObjHandle& cond,
                            const ObjHandle& in1, const ObjHandle& in2)   override;
  /* clang-format on */

protected:
  class Context;
  const Context *context;

  class Alloc;
  const Alloc *alloc;

#ifdef USE_OBJ_POOL
  using ObjMan = obj_man::Pool<Alloc>;
#else
  using ObjMan = obj_man::Basic<Alloc>;
#endif

  ObjMan *mm;
};

} // namespace cingulata

#endif
