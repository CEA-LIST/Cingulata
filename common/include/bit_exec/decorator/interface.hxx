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

#ifndef BIT_EXEC_DECORATOR_INTERFACE
#define BIT_EXEC_DECORATOR_INTERFACE

#include <bit_exec/obj_handle.hxx>
#include <utils.hxx>

#include <string>

namespace cingulata {
namespace decorator {

/**
 * @brief      Decorator interface for bit executors
 */
class IDecorator {
public:
  /* clang-format off */

  virtual void pre_init       () {}
  virtual void pre_reset      () {}

  virtual void pre_encode     (const bit_plain_t pt_val) {}
  virtual void pre_encrypt    (const bit_plain_t pt_val) {}
  virtual void pre_decrypt    (const ObjHandle& in) {}
  virtual void pre_read       (const std::string& name) {}
  virtual void pre_write      (const ObjHandle& in, const std::string& name) {}

  virtual void pre_op_not     (const ObjHandle& in) {}
  virtual void pre_op_and     (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_xor     (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_nand    (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_andyn   (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_andny   (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_or      (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_nor     (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_oryn    (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_orny    (const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void pre_op_xnor    (const ObjHandle& in1, const ObjHandle& in2) {}

  virtual void pre_op_mux     (const ObjHandle& cond,
                                const ObjHandle& in1, const ObjHandle& in2) {}

  virtual void post_init      () {}
  virtual void post_reset     () {}

  virtual void post_encode    (const ObjHandle& res_hdl, const bit_plain_t pt_val) {}
  virtual void post_encrypt   (const ObjHandle& res_hdl, const bit_plain_t pt_val) {}
  virtual void post_decrypt   (const bit_plain_t, const ObjHandle& in) {}
  virtual void post_read      (const ObjHandle& res_hdl, const std::string& name) {}
  virtual void post_write     (const ObjHandle& in, const std::string& name) {}

  virtual void post_op_not    (const ObjHandle& res_hdl, const ObjHandle& in) {}
  virtual void post_op_and    (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_xor    (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_nand   (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_andyn  (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_andny  (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_or     (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_nor    (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_oryn   (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_orny   (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}
  virtual void post_op_xnor   (const ObjHandle& res_hdl, const ObjHandle& in1, const ObjHandle& in2) {}

  virtual void post_op_mux    (const ObjHandle& res_hdl, const ObjHandle& cond,
                                const ObjHandle& in1, const ObjHandle& in2) {}

  /* clang-format on */
};

} // namespace decorator
} // namespace cingulata

#endif
