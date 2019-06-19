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

#ifndef BIT_EXEC_COUNTER_DECO
#define BIT_EXEC_COUNTER_DECO

#include <bit_exec/stat.hxx>
#include <bit_exec/interface.hxx>
#include <bit_exec/interface_fhe.hxx>
#include <bit_exec/interface_she.hxx>
#include <utils.hxx>

namespace cingulata {

namespace {
template <typename bit_exec_t, typename bit_exec_interface_t>
class StatDeco_impl;
}

/**
 * @brief      Bit executor statistics
 * @details    This class counts the number of times each abstract method of @c
 *             bit_exec_t is called. The actual implementation depends on the
 *             parent class of template parameter. Thus for classes inheriting
 *             from IBitExecSHE only XOR and AND gates will be counted.
 *
 * @tparam     bit_exec_t  Bit executor implementation to log
 * @tparam     <unnamed>   verify if given @c bit_exec_t class inherits from @c
 *                         IBitExec
 */
template <typename bit_exec_t, typename = typename std::enable_if_t<
                                   std::is_base_of_v<IBitExec, bit_exec_t>>>
class StatDeco
    : public StatDeco_impl<bit_exec_t, typename bit_exec_t::interface_type> {
public:
  template <typename... Args>
  StatDeco(Args... args)
      : StatDeco_impl<bit_exec_t, typename bit_exec_t::interface_type>(
            std::forward<Args>(args)...) {}
};

namespace {

template <typename bit_exec_t>
class StatDeco_impl<bit_exec_t, IBitExecFHE> : public bit_exec_t,
                                                  public StatFHE {
public:
  template <typename... Args>
  StatDeco_impl(Args... args) : bit_exec_t(std::forward<Args>(args)...) {
    StatFHE::reset();
    bit_exec_t::reset();
  }

  ObjHandle encode(const bit_plain_t pt_val = 0) override {
    StatFHE::encode();
    return bit_exec_t::encode(pt_val);
  }

  ObjHandle encrypt(const bit_plain_t pt_val = 0) override {
    StatFHE::encrypt();
    return bit_exec_t::encrypt(pt_val);
  }

  bit_plain_t decrypt(const ObjHandle &in) override {
    StatFHE::decrypt();
    return bit_exec_t::decrypt(in);
  }

  ObjHandle read(const std::string &name) override {
    StatFHE::read();
    return bit_exec_t::read(name);
  }

  void write(const ObjHandle &in, const std::string &name) override {
    StatFHE::write();
    bit_exec_t::write(in, name);
  }

  ObjHandle op_not(const ObjHandle &in) override {
    StatFHE::op_not();
    return bit_exec_t::op_not(in);
  }

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_and();
    return bit_exec_t::op_and(in1, in2);
  }

  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_xor();
    return bit_exec_t::op_xor(in1, in2);
  }

  ObjHandle op_nand(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_nand();
    return bit_exec_t::op_nand(in1, in2);
  }

  ObjHandle op_andyn(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_andyn();
    return bit_exec_t::op_andyn(in1, in2);
  }

  ObjHandle op_andny(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_andny();
    return bit_exec_t::op_andny(in1, in2);
  }

  ObjHandle op_or(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_or();
    return bit_exec_t::op_or(in1, in2);
  }

  ObjHandle op_nor(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_nor();
    return bit_exec_t::op_nor(in1, in2);
  }

  ObjHandle op_oryn(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_oryn();
    return bit_exec_t::op_oryn(in1, in2);
  }

  ObjHandle op_orny(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_orny();
    return bit_exec_t::op_orny(in1, in2);
  }

  ObjHandle op_xnor(const ObjHandle &in1, const ObjHandle &in2) override {
    StatFHE::op_xnor();
    return bit_exec_t::op_xnor(in1, in2);
  }
};

template <typename bit_exec_t>
class StatDeco_impl<bit_exec_t, IBitExecSHE> : public bit_exec_t,
                                                  public StatSHE {
public:
  template <typename... Args>
  StatDeco_impl(Args... args) : bit_exec_t(std::forward<Args>(args)...) {
    StatSHE::reset();
    bit_exec_t::reset();
  }

  ObjHandle encode(const bit_plain_t pt_val = 0) override {
    StatSHE::encode();
    return bit_exec_t::encode(pt_val);
  }

  ObjHandle encrypt(const bit_plain_t pt_val = 0) override {
    StatSHE::encrypt();
    return bit_exec_t::encrypt(pt_val);
  }

  bit_plain_t decrypt(const ObjHandle &in) override {
    StatSHE::decrypt();
    return bit_exec_t::decrypt(in);
  }

  ObjHandle read(const std::string &name) override {
    StatSHE::read();
    return bit_exec_t::read(name);
  }

  void write(const ObjHandle &in, const std::string &name) override {
    StatSHE::write();
    bit_exec_t::write(in, name);
  }

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) override {
    StatSHE::op_and();
    return bit_exec_t::op_and(in1, in2);
  }
  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) override {
    StatSHE::op_xor();
    return bit_exec_t::op_xor(in1, in2);
  }
};
} // namespace

} // namespace cingulata

#endif
