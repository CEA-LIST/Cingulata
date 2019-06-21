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

namespace cingulata {
namespace decorator {

/**
 * @brief      Decorator interface for bit executors
 */
class IDecorator {
public:
  virtual void pre_init() {}
  virtual void post_init() {}

  virtual void pre_reset() {}
  virtual void post_reset() {}

  virtual void pre_encode() {}
  virtual void post_encode() {}
  virtual void pre_encrypt() {}
  virtual void post_encrypt() {}
  virtual void pre_decrypt() {}
  virtual void post_decrypt() {}
  virtual void pre_read() {}
  virtual void post_read() {}
  virtual void pre_write() {}
  virtual void post_write() {}

  virtual void pre_op_not() {}
  virtual void post_op_not() {}
  virtual void pre_op_and() {}
  virtual void post_op_and() {}
  virtual void pre_op_xor() {}
  virtual void post_op_xor() {}
  virtual void pre_op_nand() {}
  virtual void post_op_nand() {}
  virtual void pre_op_andyn() {}
  virtual void post_op_andyn() {}
  virtual void pre_op_andny() {}
  virtual void post_op_andny() {}
  virtual void pre_op_or() {}
  virtual void post_op_or() {}
  virtual void pre_op_nor() {}
  virtual void post_op_nor() {}
  virtual void pre_op_oryn() {}
  virtual void post_op_oryn() {}
  virtual void pre_op_orny() {}
  virtual void post_op_orny() {}
  virtual void pre_op_xnor() {}
  virtual void post_op_xnor() {}

  virtual void pre_op_mux() {}
  virtual void post_op_mux() {}
};

} // namespace decorator
} // namespace cingulata

#endif
