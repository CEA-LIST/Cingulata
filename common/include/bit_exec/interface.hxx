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

#ifndef BIT_EXEC_INTERFACE
#define BIT_EXEC_INTERFACE

#include <bit_exec/obj_handle.hxx>
#include <utils.hxx>

#include <memory>
#include <stdint.h>
#include <string>

namespace cingulata {
/**
 * @brief Abstract interface to bit execution modules
 * @details This interface is adapted for HE schemes with boolean plaintext
 *  space, bit tracking modules, etc.
 * The inputs and outputs of interface operations are generic object handles.
 * Each operation creates a new object handle. Handles point to object
 *  created by methods \c new_obj and \c del_obj. These methods can be
 *  overloaded in a child class, otherwise the default implementation creates
 *  empty handles.
 */
class IBitExec {
public:
  /* clang-format off */

  IBitExec()          = default;
  virtual ~IBitExec() = default;

  /**
   * @brief Initialize executor
   *  This method initializes internal state of executor.
   *  It should be overloaded in child-classes with state.
   */
  virtual void        init        () {}

  /**
   * @brief Reset the internal state of executor,
   *  if it has one
   */
  virtual void        reset       () {}

  /**
   * encode, encrypt, decrypt and IO operations
   */
  virtual ObjHandle   encode      (const bit_plain_t pt_val = 0)                  = 0;
  virtual ObjHandle   encrypt     (const bit_plain_t pt_val = 0)                  = 0;
  virtual bit_plain_t decrypt     (const ObjHandle& in1)                          = 0;
  virtual ObjHandle   read        (const std::string& name)                       = 0;
  virtual void        write       (const ObjHandle& in1, const std::string& name) = 0;

  virtual ObjHandle   op_not      (const ObjHandle& in1);

  /* Only AND and XOR gates are mandatory */
  virtual ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    = 0;
  virtual ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    = 0;

  virtual ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2);

  /** Ternary operator cond?in1:in2, i.e. oblivious select */
  virtual ObjHandle   op_mux      (const ObjHandle& cond,
                                    const ObjHandle& in1, const ObjHandle& in2);

  /* clang-format on */
};
} // namespace cingulata

#endif
