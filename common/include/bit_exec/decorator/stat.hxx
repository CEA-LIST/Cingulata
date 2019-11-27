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

    printf(" %-8s: %6d\n", "not", m_not_cnt);
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "and", m_and_cnt);
    printf(" %-8s: %6d\n", "xor", m_xor_cnt);
    printf(" %-8s: %6d\n", "nand", m_nand_cnt);
    printf(" %-8s: %6d\n", "andyn", m_andyn_cnt);
    printf(" %-8s: %6d\n", "andny", m_andny_cnt);
    printf(" %-8s: %6d\n", "or", m_or_cnt);
    printf(" %-8s: %6d\n", "nor", m_nor_cnt);
    printf(" %-8s: %6d\n", "oryn", m_oryn_cnt);
    printf(" %-8s: %6d\n", "orny", m_orny_cnt);
    printf(" %-8s: %6d\n", "xnor", m_xnor_cnt);
    printf(" %-8s\n", "and");
    printf(" %-8s\n", " + xor");
    printf(" %-8s\n", " + nand");
    printf(" %-8s\n", " + andyn");
    printf(" %-8s\n", " + andny");
    printf(" %-8s\n", " + or");
    printf(" %-8s\n", " + nor");
    printf(" %-8s\n", " + oryn");
    printf(" %-8s\n", " + orny");
    printf(" %-8s= %6d\n", " + xnor", total_bin_op_cnt());
    printf(" %-16s\n", "------------------");

    printf(" %-8s: %6d\n", "mux", m_mux_cnt);
    printf(" %-16s\n", "------------------");
  }

  unsigned encode_cnt()   { return m_cnt_encode;  }
  unsigned encrypt_cnt()  { return m_cnt_encrypt; }
  unsigned decrypt_cnt()  { return m_cnt_decrypt; }
  unsigned read_cnt()     { return m_cnt_read;    }
  unsigned write_cnt()    { return m_cnt_write;   }
  unsigned not_cnt()      { return m_not_cnt;     }
  unsigned and_cnt()      { return m_and_cnt;     }
  unsigned xor_cnt()      { return m_xor_cnt;     }
  unsigned nand_cnt()     { return m_nand_cnt;    }
  unsigned andyn_cnt()    { return m_andyn_cnt;   }
  unsigned andny_cnt()    { return m_andny_cnt;   }
  unsigned or_cnt()       { return m_or_cnt;      }
  unsigned nor_cnt()      { return m_nor_cnt;     }
  unsigned oryn_cnt()     { return m_oryn_cnt;    }
  unsigned orny_cnt()     { return m_orny_cnt;    }
  unsigned xnor_cnt()     { return m_xnor_cnt;    }
  unsigned mux_cnt()      { return m_mux_cnt;     }

  unsigned total_bin_op_cnt() {
    return m_and_cnt + m_xor_cnt + m_nand_cnt + m_andyn_cnt +
           m_andny_cnt + m_or_cnt + m_nor_cnt + m_oryn_cnt +
           m_orny_cnt + m_xnor_cnt;
  }

  void post_reset() override {
    m_cnt_encode = 0;
    m_cnt_encrypt = 0;
    m_cnt_decrypt = 0;
    m_cnt_read = 0;
    m_cnt_write = 0;
    m_not_cnt = 0;
    m_and_cnt = 0;
    m_xor_cnt = 0;
    m_nand_cnt = 0;
    m_andyn_cnt = 0;
    m_andny_cnt = 0;
    m_or_cnt = 0;
    m_nor_cnt = 0;
    m_oryn_cnt = 0;
    m_orny_cnt = 0;
    m_xnor_cnt = 0;
    m_mux_cnt = 0;
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
    m_not_cnt++;
  }

  void post_op_and(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_and_cnt++;
  }

  void post_op_xor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_xor_cnt++;
  }

  void post_op_nand(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_nand_cnt++;
  }

  void post_op_andyn(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {
    m_andyn_cnt++;
  }

  void post_op_andny(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {
    m_andny_cnt++;
  }

  void post_op_or(const ObjHandle &, const ObjHandle &,
                  const ObjHandle &) override {
    m_or_cnt++;
  }

  void post_op_nor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_nor_cnt++;
  }

  void post_op_oryn(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_oryn_cnt++;
  }

  void post_op_orny(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_orny_cnt++;
  }

  void post_op_xnor(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {
    m_xnor_cnt++;
  }

  void post_op_mux(const ObjHandle &, const ObjHandle &cond, const ObjHandle &,
                   const ObjHandle &) override {
    m_mux_cnt++;
  }

protected:
  unsigned m_cnt_encode;
  unsigned m_cnt_encrypt;
  unsigned m_cnt_decrypt;
  unsigned m_cnt_read;
  unsigned m_cnt_write;
  unsigned m_not_cnt;
  unsigned m_and_cnt;
  unsigned m_xor_cnt;
  unsigned m_nand_cnt;
  unsigned m_andyn_cnt;
  unsigned m_andny_cnt;
  unsigned m_or_cnt;
  unsigned m_nor_cnt;
  unsigned m_oryn_cnt;
  unsigned m_orny_cnt;
  unsigned m_xnor_cnt;
  unsigned m_mux_cnt;
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

    printf(" %-8s: %6d\n", "and", m_and_cnt);
    printf(" %-8s: %6d\n", "xor", m_xor_cnt);
    printf(" %-16s\n", "------------------");
  }

  unsigned encode_cnt()   { return m_cnt_encode;  }
  unsigned encrypt_cnt()  { return m_cnt_encrypt; }
  unsigned decrypt_cnt()  { return m_cnt_decrypt; }
  unsigned read_cnt()     { return m_cnt_read;    }
  unsigned write_cnt()    { return m_cnt_write;   }
  unsigned and_cnt()      { return m_and_cnt;     }
  unsigned xor_cnt()      { return m_xor_cnt;     }

  void post_reset() override {
    m_cnt_encode = 0;
    m_cnt_encrypt = 0;
    m_cnt_decrypt = 0;
    m_cnt_read = 0;
    m_cnt_write = 0;
    m_and_cnt = 0;
    m_xor_cnt = 0;
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
    m_and_cnt++;
  }

  void post_op_xor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {
    m_xor_cnt++;
  }

protected:
  unsigned m_cnt_encode;
  unsigned m_cnt_encrypt;
  unsigned m_cnt_decrypt;
  unsigned m_cnt_read;
  unsigned m_cnt_write;
  unsigned m_and_cnt;
  unsigned m_xor_cnt;
};
} // namespace

} // namespace decorator
} // namespace cingulata

#endif
