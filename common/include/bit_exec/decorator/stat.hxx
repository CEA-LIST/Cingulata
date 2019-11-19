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

/**
 * @brief      Bit executor statistics decorator
 * @details    This class counts the number of times each abstract method of @c
 *             bit_exec_interface_t is called. The actual implementation depends
 *             on the parent class of template parameter. Thus for classes
 *             inheriting from IBitExecSHE only XOR and AND gates will be
 *             counted.
 *
 * @tparam     bit_exec_interface_t  Bit executor implementation to log
 */
template <typename bit_exec_interface_t> class Stat : public IDecorator {
public:
  Stat() { post_reset(); }

  void print() {}

  unsigned encode_cnt();
  unsigned encrypt_cnt();
  unsigned decrypt_cnt();
  unsigned read_cnt();
  unsigned write_cnt();

  unsigned not_cnt();
  unsigned and_cnt();
  unsigned xor_cnt();
  unsigned nand_cnt();
  unsigned andyn_cnt();
  unsigned andny_cnt();
  unsigned or_cnt();
  unsigned nor_cnt();
  unsigned oryn_cnt();
  unsigned orny_cnt();
  unsigned xnor_cnt();
  unsigned mux_cnt();

  unsigned total_bin_op_cnt();

  void post_reset() override {
    m_encode_cnt = 0;
    m_encrypt_cnt = 0;
    m_decrypt_cnt = 0;
    m_read_cnt = 0;
    m_write_cnt = 0;
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

  void post_encode(const ObjHandle &, const bit_plain_t) override;

  void post_encrypt(const ObjHandle &, const bit_plain_t) override;

  void post_decrypt(const bit_plain_t, const ObjHandle &) override;

  void post_read(const ObjHandle &, const std::string &) override;

  void post_write(const ObjHandle &, const std::string &) override;

  void post_op_not(const ObjHandle &, const ObjHandle &) override {}

  void post_op_and(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override;

  void post_op_xor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override;

  void post_op_nand(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {}

  void post_op_andyn(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {}

  void post_op_andny(const ObjHandle &, const ObjHandle &,
                     const ObjHandle &) override {}

  void post_op_or(const ObjHandle &, const ObjHandle &,
                  const ObjHandle &) override {}

  void post_op_nor(const ObjHandle &, const ObjHandle &,
                   const ObjHandle &) override {}

  void post_op_oryn(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {}

  void post_op_orny(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {}

  void post_op_xnor(const ObjHandle &, const ObjHandle &,
                    const ObjHandle &) override {}

  void post_op_mux(const ObjHandle &, const ObjHandle &cond, const ObjHandle &,
                   const ObjHandle &) override {}

private:
  unsigned m_encode_cnt;
  unsigned m_encrypt_cnt;
  unsigned m_decrypt_cnt;
  unsigned m_read_cnt;
  unsigned m_write_cnt;
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

template <> void Stat<IBitExecFHE>::print() {
  printf("Number of executions:\n");
  printf(" %-8s: %6d\n", "encode", m_encode_cnt);
  printf(" %-8s: %6d\n", "encrypt", m_encrypt_cnt);
  printf(" %-8s: %6d\n", "decrypt", m_decrypt_cnt);
  printf(" %-8s: %6d\n", "read", m_read_cnt);
  printf(" %-8s: %6d\n", "write", m_write_cnt);
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
  unsigned total_bin_op_cnt = m_and_cnt + m_xor_cnt + m_nand_cnt + m_andyn_cnt +
                              m_andny_cnt + m_or_cnt + m_nor_cnt + m_oryn_cnt +
                              m_orny_cnt + m_xnor_cnt;
  printf(" %-8s= %6d\n", " + xnor", total_bin_op_cnt);
  printf(" %-16s\n", "------------------");

  printf(" %-8s: %6d\n", "mux", m_mux_cnt);
  printf(" %-16s\n", "------------------");
}

template <> void Stat<IBitExecSHE>::print() {
  // StatGroup<IBitExecSHE>(make_shared<Stat<IBitExecSHE>>(*this)).print();

  printf("Number of executions:\n");
  printf(" %-8s: %6d\n", "encode", m_encode_cnt);
  printf(" %-8s: %6d\n", "encrypt", m_encrypt_cnt);
  printf(" %-8s: %6d\n", "decrypt", m_decrypt_cnt);
  printf(" %-8s: %6d\n", "read", m_read_cnt);
  printf(" %-8s: %6d\n", "write", m_write_cnt);
  printf(" %-16s\n", "------------------");

  printf(" %-8s: %6d\n", "and", m_and_cnt);
  printf(" %-8s: %6d\n", "xor", m_xor_cnt);
  printf(" %-16s\n", "------------------");
}

template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::encode_cnt() {
  return m_encode_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::encrypt_cnt() {
  return m_encrypt_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::decrypt_cnt() {
  return m_decrypt_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::read_cnt() {
  return m_read_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::write_cnt() {
  return m_write_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::and_cnt() {
  return m_and_cnt;
}
template <typename bit_exec_interface_t>
unsigned Stat<bit_exec_interface_t>::xor_cnt() {
  return m_xor_cnt;
}

template <> unsigned Stat<IBitExecFHE>::not_cnt() { return m_not_cnt; }
template <> unsigned Stat<IBitExecFHE>::nand_cnt() { return m_nand_cnt; }
template <> unsigned Stat<IBitExecFHE>::andyn_cnt() { return m_andyn_cnt; }
template <> unsigned Stat<IBitExecFHE>::andny_cnt() { return m_andny_cnt; }
template <> unsigned Stat<IBitExecFHE>::or_cnt() { return m_or_cnt; }
template <> unsigned Stat<IBitExecFHE>::nor_cnt() { return m_nor_cnt; }
template <> unsigned Stat<IBitExecFHE>::oryn_cnt() { return m_oryn_cnt; }
template <> unsigned Stat<IBitExecFHE>::orny_cnt() { return m_orny_cnt; }
template <> unsigned Stat<IBitExecFHE>::xnor_cnt() { return m_xnor_cnt; }
template <> unsigned Stat<IBitExecFHE>::mux_cnt() { return m_mux_cnt; }

template <> unsigned Stat<IBitExecFHE>::total_bin_op_cnt() {
  return m_and_cnt + m_xor_cnt + m_nand_cnt + m_andyn_cnt + m_andny_cnt +
         m_or_cnt + m_nor_cnt + m_oryn_cnt + m_orny_cnt + m_xnor_cnt;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_encode(const ObjHandle &,
                                             const bit_plain_t) {
  m_encode_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_encrypt(const ObjHandle &,
                                              const bit_plain_t) {
  m_encrypt_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_decrypt(const bit_plain_t,
                                              const ObjHandle &) {
  m_decrypt_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_read(const ObjHandle &,
                                           const std::string &) {
  m_read_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_write(const ObjHandle &,
                                            const std::string &) {
  m_write_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_op_and(const ObjHandle &,
                                             const ObjHandle &,
                                             const ObjHandle &) {
  m_and_cnt++;
}

template <typename bit_exec_interface_t>
void Stat<bit_exec_interface_t>::post_op_xor(const ObjHandle &,
                                             const ObjHandle &,
                                             const ObjHandle &) {
  m_xor_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_nand(const ObjHandle &, const ObjHandle &,
                                     const ObjHandle &) {
  m_nand_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_andyn(const ObjHandle &, const ObjHandle &,
                                      const ObjHandle &) {
  m_andyn_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_andny(const ObjHandle &, const ObjHandle &,
                                      const ObjHandle &) {
  m_andny_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_or(const ObjHandle &, const ObjHandle &,
                                   const ObjHandle &) {
  m_or_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_nor(const ObjHandle &, const ObjHandle &,
                                    const ObjHandle &) {
  m_nor_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_oryn(const ObjHandle &, const ObjHandle &,
                                     const ObjHandle &) {
  m_oryn_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_orny(const ObjHandle &, const ObjHandle &,
                                     const ObjHandle &) {
  m_orny_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_xnor(const ObjHandle &, const ObjHandle &,
                                     const ObjHandle &) {
  m_xnor_cnt++;
}

template <>
void Stat<IBitExecFHE>::post_op_mux(const ObjHandle &, const ObjHandle &cond,
                                    const ObjHandle &, const ObjHandle &) {
  m_mux_cnt++;
}

} // namespace decorator
} // namespace cingulata

#endif
