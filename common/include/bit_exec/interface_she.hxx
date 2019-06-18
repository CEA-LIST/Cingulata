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

#ifndef BIT_EXEC_INTERFACE_SHE
#define BIT_EXEC_INTERFACE_SHE

#include <bit_exec/interface.hxx>

#include <memory>
#include <stdint.h>
#include <string>

namespace cingulata {

/**
 * @brief      Abstract interface to somewhat HE schemes with boolean plaintext
 *             space.
 */
class IBitExecSHE : public IBitExec {
public:
  /* clang-format off */

  IBitExecSHE()          = default;
  virtual ~IBitExecSHE() = default;

  /* Only AND and XOR gates are mandatory in SHE schemes */
  virtual ObjHandle   op_not      (const ObjHandle& in1)                        override;
  virtual ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2)  override;
  virtual ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2)  override;

  /** Ternary operator cond?in1:in2, i.e. oblivious select */
  virtual ObjHandle   op_mux      (const ObjHandle& cond,
                                    const ObjHandle& in1, const ObjHandle& in2) override;

  /* clang-format on */
};

} // namespace cingulata

#endif
