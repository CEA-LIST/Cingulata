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

#ifndef BIT_EXEC_STAT
#define BIT_EXEC_STAT

#include <bit_exec/interface.hxx>
#include <bit_exec/interface_fhe.hxx>
#include <bit_exec/interface_she.hxx>
#include <utils.hxx>

#include <memory>
#include <stdint.h>
#include <string>

namespace cingulata {
/**
 * @brief      Abstract interface to bit execution modules
 * @details    This interface is adapted for HE schemes with boolean plaintext
 *             space, bit tracking modules, etc. The inputs and outputs of
 *             interface operations are generic object handles. Each operation
 *             creates a new object handle.
 */
template <typename bit_exec_t,
  typename = typename std::enable_if_t<std::is_base_of_v<IBitExecFHE, bit_exec_t>>>
class Stat : public bit_exec_t {
public:
  template<typename... Args>
  Stat(Args... args) : bit_exec_t(std::forward<Args>(args)...) {
    reset();
  }

  void print() {
    printf("#%10s: %6d\n", "encode", m_cnt_encode);
    printf("#%10s: %6d\n", "encrypt", m_cnt_encrypt);
    printf("#%10s: %6d\n", "decrypt", m_cnt_decrypt);
    printf("#%10s: %6d\n", "read", m_cnt_read);
    printf("#%10s: %6d\n", "write", m_cnt_write);
    printf("#%10s: %6d\n", "op_not", m_cnt_op_not);
    printf("#%10s: %6d\n", "op_and", m_cnt_op_and);
    printf("#%10s: %6d\n", "op_xor", m_cnt_op_xor);
    printf("#%10s: %6d\n", "op_nand", m_cnt_op_nand);
    printf("#%10s: %6d\n", "op_andyn", m_cnt_op_andyn);
    printf("#%10s: %6d\n", "op_andny", m_cnt_op_andny);
    printf("#%10s: %6d\n", "op_or", m_cnt_op_or);
    printf("#%10s: %6d\n", "op_nor", m_cnt_op_nor);
    printf("#%10s: %6d\n", "op_oryn", m_cnt_op_oryn);
    printf("#%10s: %6d\n", "op_orny", m_cnt_op_orny);
    printf("#%10s: %6d\n", "op_xnor", m_cnt_op_xnor);
    printf("#%10s: %6d\n", "op_mux", m_cnt_op_mux);
  }

  void reset() override {
    m_cnt_encode   = 0;
    m_cnt_encrypt  = 0;
    m_cnt_decrypt  = 0;
    m_cnt_read     = 0;
    m_cnt_write    = 0;
    m_cnt_op_not   = 0;
    m_cnt_op_and   = 0;
    m_cnt_op_xor   = 0;
    m_cnt_op_nand  = 0;
    m_cnt_op_andyn = 0;
    m_cnt_op_andny = 0;
    m_cnt_op_or    = 0;
    m_cnt_op_nor   = 0;
    m_cnt_op_oryn  = 0;
    m_cnt_op_orny  = 0;
    m_cnt_op_xnor  = 0;
    m_cnt_op_mux   = 0;
  }

  ObjHandle encode(const bit_plain_t pt_val = 0) override {
    m_cnt_encode++;
    return bit_exec_t::encode(pt_val);
  }

  ObjHandle encrypt(const bit_plain_t pt_val = 0) override {
    m_cnt_encrypt++;
    return bit_exec_t::encrypt(pt_val);
  }

  bit_plain_t decrypt(const ObjHandle &in) override {
    m_cnt_decrypt++;
    return bit_exec_t::decrypt(in);
  }

  ObjHandle read(const std::string &name) override {
    m_cnt_read++;
    return bit_exec_t::read(name);
  }

  void write(const ObjHandle &in, const std::string &name) override {
    m_cnt_write++;
    bit_exec_t::write(in, name);
  }

  ObjHandle op_not(const ObjHandle &in) override {
    m_cnt_op_not++;
    return bit_exec_t::op_not(in);
  }

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_and++;
    return bit_exec_t::op_and(in1, in2);
  }

  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_xor++;
    return bit_exec_t::op_xor(in1, in2);
  }

  ObjHandle op_nand(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_nand++;
    return bit_exec_t::op_nand(in1, in2);
  }

  ObjHandle op_andyn(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_andyn++;
    return bit_exec_t::op_andyn(in1, in2);
  }

  ObjHandle op_andny(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_andny++;
    return bit_exec_t::op_andny(in1, in2);
  }

  ObjHandle op_or(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_or++;
    return bit_exec_t::op_or(in1, in2);
  }

  ObjHandle op_nor(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_nor++;
    return bit_exec_t::op_nor(in1, in2);
  }

  ObjHandle op_oryn(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_oryn++;
    return bit_exec_t::op_oryn(in1, in2);
  }

  ObjHandle op_orny(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_orny++;
    return bit_exec_t::op_orny(in1, in2);
  }

  ObjHandle op_xnor(const ObjHandle &in1, const ObjHandle &in2) override {
    m_cnt_op_xnor++;
    return bit_exec_t::op_xnor(in1, in2);
  }

protected:
  unsigned m_cnt_encode;
  unsigned m_cnt_encrypt;
  unsigned m_cnt_decrypt;
  unsigned m_cnt_read;
  unsigned m_cnt_write;
  unsigned m_cnt_op_not;
  unsigned m_cnt_op_and;
  unsigned m_cnt_op_xor;
  unsigned m_cnt_op_nand;
  unsigned m_cnt_op_andyn;
  unsigned m_cnt_op_andny;
  unsigned m_cnt_op_or;
  unsigned m_cnt_op_nor;
  unsigned m_cnt_op_oryn;
  unsigned m_cnt_op_orny;
  unsigned m_cnt_op_xnor;
  unsigned m_cnt_op_mux;
};

} // namespace cingulata

#endif
