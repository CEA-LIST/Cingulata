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

#ifndef BIT_EXEC_PLAIN
#define BIT_EXEC_PLAIN

#include <bit_exec/interface.hxx>
#include <bit_exec/obj_man/allocator.hxx>
#include <bit_exec/obj_man/basic.hxx>

namespace cingulata {

/**
 * @brief      Implementation of @c IBitExec for clear-text execution
 * @details    This class can be used to execute the same operations on several
 *             bits (each bit in @c bit_plain_t type) in parallel, aka SIMD or
 *             bit-sliced execution.
 */
class BitExecClear : public IBitExec {
public:
  /**
   * @brief      Constructs an object handle containing value @c pt_val
   *
   * @param[in]  pt_val  The plaintext value
   *
   * @return     Handle to plaintext value
   */
  ObjHandle encode(const bit_plain_t pt_val) override;

  /** @copybrief encode
   */
  ObjHandle encrypt(const bit_plain_t pt_val) override;

  /**
   * @brief      Gets plaintext value contained in handle @c in
   *
   * @param[in]  in    Object handle
   *
   * @return     The handled plaintext value
   */
  bit_plain_t decrypt(const ObjHandle &in) override;

  /**
   * @brief      Read value from file named @c name and build a handle over this
   * value
   *
   * @param[in]  name  File name
   *
   * @return     Handle to plaintext value from file
   */
  ObjHandle read(const std::string &name) override;

  /**
   * @brief      Write handled plaintext value to file named @c name
   *
   * @param[in]  in    Plaintext value handle
   * @param[in]  name  File name
   */
  void write(const ObjHandle &in, const std::string &name) override;

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) override;
  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) override;

protected:
  obj_man::Basic<obj_man::Allocator<bit_plain_t>> mm;
};

} // namespace cingulata

#endif
