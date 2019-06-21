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

#ifndef BIT_EXEC_DECORATOR_DEPTH
#define BIT_EXEC_DECORATOR_DEPTH

#include <bit_exec/decorator/interface.hxx>

#include <cstdio>
#include <unordered_map>

namespace cingulata {

class IBitExecFHE;
class IBitExecSHE;

namespace decorator {

namespace {
template <typename bit_exec_interface_t> class Depth_impl;
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
class Depth : public Depth_impl<typename bit_exec_t::interface_type> {};

namespace {

template <> class Depth_impl<IBitExecSHE> : public IDecorator {
  struct DepthVals {
    unsigned mult_depth;
    unsigned depth;
  };

public:
  Depth_impl() { post_reset(); }

  void print() {
    printf("Multiplicative depth: %6d\n", m_mult_depth_max);
    printf("Overall depth       : %6d\n", m_depth_max);
  }

  void post_reset() override {
    m_depth.clear();
    m_mult_depth_max = 0;
    m_depth_max = 0;
  }

  void post_encode(const ObjHandle &res, const bit_plain_t) override {
    m_depth[res.get<void>()] = {0, 0};
  }

  void post_encrypt(const ObjHandle &res, const bit_plain_t) override {
    m_depth[res.get<void>()] = {0, 0};
  }

  void post_read(const ObjHandle &res, const std::string &) override {
    m_depth[res.get<void>()] = {0, 0};
  }

  void post_op_and(const ObjHandle &res, const ObjHandle &in1,
                   const ObjHandle &in2) override {
    const DepthVals &vals_1 = m_depth.at(in1.get<void>());
    const DepthVals &vals_2 = m_depth.at(in2.get<void>());

    const unsigned mult_depth =
        std::max(vals_1.mult_depth, vals_2.mult_depth) + 1;
    const unsigned depth = std::max(vals_1.depth, vals_2.depth) + 1;

    m_depth[res.get<void>()] = {mult_depth, depth};
    m_mult_depth_max = std::max(m_mult_depth_max, mult_depth);
    m_depth_max = std::max(m_depth_max, depth);
  }

  void post_op_xor(const ObjHandle &res, const ObjHandle &in1,
                   const ObjHandle &in2) override {
    const DepthVals &vals_1 = m_depth.at(in1.get<void>());
    const DepthVals &vals_2 = m_depth.at(in2.get<void>());

    const unsigned mult_depth = std::max(vals_1.mult_depth, vals_2.mult_depth);
    const unsigned depth = std::max(vals_1.depth, vals_2.depth) + 1;

    m_depth[res.get<void>()] = {mult_depth, depth};
    m_mult_depth_max = std::max(m_mult_depth_max, mult_depth);
    m_depth_max = std::max(m_depth_max, depth);
  }

protected:
  /**
   * map from object handle pointer to tuple of multiplicative and overall depth
   */
  std::unordered_map<void *, DepthVals> m_depth;
  unsigned m_mult_depth_max;
  unsigned m_depth_max;
};
} // namespace

} // namespace decorator
} // namespace cingulata

#endif
