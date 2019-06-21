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

#ifndef BIT_EXEC_DECORATOR_ATTACH
#define BIT_EXEC_DECORATOR_ATTACH

#include <bit_exec/obj_handle.hxx>
#include <utils.hxx>

#include <string>

namespace cingulata {
namespace decorator {

namespace {
/**
 * @brief      Attaches "pre" and "post" execution functions of class @c
 *             deco_impl_t to virtual functions of bit executor class @c
 *             bit_exec_t.
 *
 * @tparam     bit_exec_t   Bit executor
 * @tparam     deco_impl_t  Decorator
 */
template <typename bit_exec_t, typename deco_impl_t> class Attach_inner;
} // namespace

/**
 * @brief      Attach several decorators to a bit executor.
 *
 * @tparam     bit_exec_t   Bit executor
 * @tparam     deco_impl_t  First decorator
 * @tparam     Types        Other decorators
 */
template <typename bit_exec_t, template <typename> class deco_impl_t,
          template <typename> class... Types>
class Attach : public Attach_inner<Attach<bit_exec_t, Types...>,
                                  deco_impl_t<bit_exec_t>> {
public:
  template <typename... Args>
  Attach(Args... args)
      : Attach_inner<Attach<bit_exec_t, Types...>, deco_impl_t<bit_exec_t>>(
            std::forward<Args>(args)...) {}
};


/**
 * @brief      Specialization of #Attach class to one decorator
 */
template <typename bit_exec_t, template <typename> class deco_impl_t>
class Attach<bit_exec_t, deco_impl_t>
    : public Attach_inner<bit_exec_t, deco_impl_t<bit_exec_t>> {
public:
  template <typename... Args>
  Attach(Args... args)
      : Attach_inner<bit_exec_t, deco_impl_t<bit_exec_t>>(
            std::forward<Args>(args)...) {}
};

namespace {

template <typename bit_exec_t, typename deco_impl_t>
class Attach_inner : public bit_exec_t, public deco_impl_t {
public:
  template <typename... Args>
  Attach_inner(Args... args) : bit_exec_t(std::forward<Args>(args)...) {}

  void init() {
    deco_impl_t::pre_init();
    bit_exec_t::init();
    deco_impl_t::post_init();
  }

  void reset() {
    deco_impl_t::pre_reset();
    bit_exec_t::reset();
    deco_impl_t::post_reset();
  }

  ObjHandle encode(const bit_plain_t pt_val) {
    deco_impl_t::pre_encode();
    auto res = bit_exec_t::encode(pt_val);
    deco_impl_t::post_encode();
    return res;
  }

  ObjHandle encrypt(const bit_plain_t pt_val) {
    deco_impl_t::pre_encrypt();
    auto res = bit_exec_t::encrypt(pt_val);
    deco_impl_t::post_encrypt();
    return res;
  }

  bit_plain_t decrypt(const ObjHandle &in1) {
    deco_impl_t::pre_decrypt();
    auto res = bit_exec_t::decrypt(in1);
    deco_impl_t::post_decrypt();
    return res;
  }

  ObjHandle read(const std::string &name) {
    deco_impl_t::pre_read();
    auto res = bit_exec_t::read(name);
    deco_impl_t::post_read();
    return res;
  }

  void write(const ObjHandle &in1, const std::string &name) {
    deco_impl_t::pre_write();
    bit_exec_t::write(in1, name);
    deco_impl_t::post_write();
  }

  ObjHandle op_not(const ObjHandle &in1) {
    deco_impl_t::pre_op_not();
    auto res = bit_exec_t::op_not(in1);
    deco_impl_t::post_op_not();
    return res;
  }

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_and();
    auto res = bit_exec_t::op_and(in1, in2);
    deco_impl_t::post_op_and();
    return res;
  }

  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_xor();
    auto res = bit_exec_t::op_xor(in1, in2);
    deco_impl_t::post_op_xor();
    return res;
  }

  ObjHandle op_nand(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_nand();
    auto res = bit_exec_t::op_nand(in1, in2);
    deco_impl_t::post_op_nand();
    return res;
  }

  ObjHandle op_andyn(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_andyn();
    auto res = bit_exec_t::op_andyn(in1, in2);
    deco_impl_t::post_op_andyn();
    return res;
  }

  ObjHandle op_andny(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_andny();
    auto res = bit_exec_t::op_andny(in1, in2);
    deco_impl_t::post_op_andny();
    return res;
  }

  ObjHandle op_or(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_or();
    auto res = bit_exec_t::op_or(in1, in2);
    deco_impl_t::post_op_or();
    return res;
  }

  ObjHandle op_nor(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_nor();
    auto res = bit_exec_t::op_nor(in1, in2);
    deco_impl_t::post_op_nor();
    return res;
  }

  ObjHandle op_oryn(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_oryn();
    auto res = bit_exec_t::op_oryn(in1, in2);
    deco_impl_t::post_op_oryn();
    return res;
  }

  ObjHandle op_orny(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_orny();
    auto res = bit_exec_t::op_orny(in1, in2);
    deco_impl_t::post_op_orny();
    return res;
  }

  ObjHandle op_xnor(const ObjHandle &in1, const ObjHandle &in2) {
    deco_impl_t::pre_op_xnor();
    auto res = bit_exec_t::op_xnor(in1, in2);
    deco_impl_t::post_op_xnor();
    return res;
  }

  ObjHandle op_mux(const ObjHandle &cond, const ObjHandle &in1,
                   const ObjHandle &in2) {
    deco_impl_t::pre_op_mux();
    auto res = bit_exec_t::op_mux(cond, in1, in2);
    deco_impl_t::post_op_mux();
    return res;
  }
};
} // namespace

} // namespace decorator
} // namespace cingulata

#endif
