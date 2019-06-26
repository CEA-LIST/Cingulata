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

#ifndef BIT_EXEC_DECORATOR_STAT
#define BIT_EXEC_DECORATOR_STAT

#include <bit_exec/decorator/interface.hxx>

#include <cstdio>

namespace cingulata {

class IBitExecFHE;
class IBitExecSHE;

namespace decorator {

namespace {
template <typename bit_exec_interface_t> class Stat_impl;
};

/**
 * @brief      Bit executor statistics decorator
 * @details    This class counts the number of times each abstract method of @c
 *             bit_exec_t is called. The actual implementation depends on the
 *             parent class of template parameter. Thus for classes inheriting
 *             from IBitExecSHE only XOR and AND gates will be counted.
 *
 * @tparam     bit_exec_t  Bit executor implementation to log
 * @tparam     <unnamed>   verify if given @c bit_exec_t class inherits from @c
 *                         IBitExec
 */
template <typename bit_exec_t>
class Stat : public Stat_impl<typename bit_exec_t::interface_type> {};

namespace {

template <> class Stat_impl<IBitExecFHE> : public IDecorator {
public:
  Stat_impl() { post_reset(); }

  void print() {
    printf("Number of executions:\n");
    printf(" %-8s: %6d\n", "encode", m_cnt_encode);
    printf(" %-8s: %6d\n", "encrypt", m_cnt_encrypt);
    printf(" %-8s: %6d\n", "decrypt", m_cnt_decrypt);
    printf(" %-8s: %6d\n", "read", m_cnt_read);
    printf(" %-8s: %6d\n", "write", m_cnt_write);
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "not", m_cnt_op_not);
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "and", m_cnt_op_and);
    printf(" %-8s: %6d\n", "xor", m_cnt_op_xor);
    printf(" %-8s: %6d\n", "nand", m_cnt_op_nand);
    printf(" %-8s: %6d\n", "andyn", m_cnt_op_andyn);
    printf(" %-8s: %6d\n", "andny", m_cnt_op_andny);
    printf(" %-8s: %6d\n", "or", m_cnt_op_or);
    printf(" %-8s: %6d\n", "nor", m_cnt_op_nor);
    printf(" %-8s: %6d\n", "oryn", m_cnt_op_oryn);
    printf(" %-8s: %6d\n", "orny", m_cnt_op_orny);
    printf(" %-8s: %6d\n", "xnor", m_cnt_op_xnor);
    unsigned total = m_cnt_op_and + m_cnt_op_xor + m_cnt_op_nand +
                     m_cnt_op_andyn + m_cnt_op_andny + m_cnt_op_or +
                     m_cnt_op_nor + m_cnt_op_oryn + m_cnt_op_orny +
                     m_cnt_op_xnor;
    printf(" %-8s\n", "and");
    printf(" %-8s\n", " + xor");
    printf(" %-8s\n", " + nand");
    printf(" %-8s\n", " + andyn");
    printf(" %-8s\n", " + andny");
    printf(" %-8s\n", " + or");
    printf(" %-8s\n", " + nor");
    printf(" %-8s\n", " + oryn");
    printf(" %-8s\n", " + orny");
    printf(" %-8s= %6d\n", " + xnor", total);
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "mux", m_cnt_op_mux);
    printf(" %-16s\n", "------------------");
  }

  void post_reset() override {
    m_cnt_encode = 0;
    m_cnt_encrypt = 0;
    m_cnt_decrypt = 0;
    m_cnt_read = 0;
    m_cnt_write = 0;
    m_cnt_op_not = 0;
    m_cnt_op_and = 0;
    m_cnt_op_xor = 0;
    m_cnt_op_nand = 0;
    m_cnt_op_andyn = 0;
    m_cnt_op_andny = 0;
    m_cnt_op_or = 0;
    m_cnt_op_nor = 0;
    m_cnt_op_oryn = 0;
    m_cnt_op_orny = 0;
    m_cnt_op_xnor = 0;
    m_cnt_op_mux = 0;
  }

  void post_encode(const ObjHandle &, const bit_plain_t) override {
    m_cnt_encode++;
  }

  void post_encrypt(const ObjHandle &, const bit_plain_t) override {
    m_cnt_encrypt++;
  }

  void post_decrypt(const bit_plain_t, const ObjHandle &) override {
    m_cnt_decrypt++;
  }

  void post_read(const ObjHandle &, const std::string &) override {
    m_cnt_read++;
  }

  void post_write(const ObjHandle &, const std::string &) override {
    m_cnt_write++;
  }

  void post_op_not(const ObjHandle &, const ObjHandle &) override {
    m_cnt_op_not++;
  }

  void post_op_and(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_and++;
  }

  void post_op_xor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_xor++;
  }

  void post_op_nand(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_cnt_op_nand++;
  }

  void post_op_andyn(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {
    m_cnt_op_andyn++;
  }

  void post_op_andny(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {
    m_cnt_op_andny++;
  }

  void post_op_or(const ObjHandle &, const ObjHandle &,
                  const ObjHandle &) override {
    m_cnt_op_or++;
  }

  void post_op_nor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_nor++;
  }

  void post_op_oryn(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_cnt_op_oryn++;
  }

  void post_op_orny(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_cnt_op_orny++;
  }

  void post_op_xnor(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_cnt_op_xnor++;
  }

  void post_op_mux(const ObjHandle &, const ObjHandle &cond, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_mux++;
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

template <> class Stat_impl<IBitExecSHE> : public IDecorator {
public:
  Stat_impl() { post_reset(); }

  void print() {
    printf("Number of executions:\n");
    printf(" %-8s: %6d\n", "encode", m_cnt_encode);
    printf(" %-8s: %6d\n", "encrypt", m_cnt_encrypt);
    printf(" %-8s: %6d\n", "decrypt", m_cnt_decrypt);
    printf(" %-8s: %6d\n", "read", m_cnt_read);
    printf(" %-8s: %6d\n", "write", m_cnt_write);
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "and", m_cnt_op_and);
    printf(" %-8s: %6d\n", "xor", m_cnt_op_xor);
    printf(" %-16s\n", "------------------");
  }

  void post_reset() override {
    m_cnt_encode = 0;
    m_cnt_encrypt = 0;
    m_cnt_decrypt = 0;
    m_cnt_read = 0;
    m_cnt_write = 0;
    m_cnt_op_and = 0;
    m_cnt_op_xor = 0;
  }

  void post_encode(const ObjHandle &, const bit_plain_t) override {
    m_cnt_encode++;
  }

  void post_encrypt(const ObjHandle &, const bit_plain_t) override {
    m_cnt_encrypt++;
  }

  void post_decrypt(const bit_plain_t, const ObjHandle &) override {
    m_cnt_decrypt++;
  }

  void post_read(const ObjHandle &, const std::string &) override {
    m_cnt_read++;
  }

  void post_write(const ObjHandle &, const std::string &) override {
    m_cnt_write++;
  }

  void post_op_and(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_and++;
  }

  void post_op_xor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_cnt_op_xor++;
  }

protected:
  unsigned m_cnt_encode;
  unsigned m_cnt_encrypt;
  unsigned m_cnt_decrypt;
  unsigned m_cnt_read;
  unsigned m_cnt_write;
  unsigned m_cnt_op_and;
  unsigned m_cnt_op_xor;
};
} // namespace

} // namespace decorator
} // namespace cingulata

#endif
