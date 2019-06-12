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

#ifndef BIT_TRACKER
#define BIT_TRACKER

#include <bit_exec/interface.hxx>
#include <bit_exec/obj_man/allocator.hxx>
#include <bit_exec/obj_man/basic.hxx>

#include <iostream>
#include <string>
#include <vector>

namespace cingulata {
namespace BitTrackerInternal {
class Node;
enum class NodeType : uint8_t;
enum class GateType : uint8_t;
} // namespace BitTrackerInternal
namespace BTI = BitTrackerInternal;

/**
 * @brief Implemenation class for bit execution interface which tracks
 *  bit operations. This class constructs a boolean circuit corresponding
 *  to every called interface operation.
 */
class BitTracker : public IBitExec {
public:
  ~BitTracker() override;

  /* clang-format off */

  /**
   * @brief Delete the boolean circuit constructed so far
   */
  void        reset       ()                                              override;

  ObjHandle   encode      (const bit_plain_t pt_val)                      override;
  ObjHandle   encrypt     (const bit_plain_t pt_val)                      override;
  bit_plain_t decrypt     (const ObjHandle& in1)                          override;
  ObjHandle   read        (const std::string& name)                       override;
  void        write       (const ObjHandle& in1, const std::string& name) override;

  ObjHandle   op_not      (const ObjHandle& in1)                          override;
  ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    override;

  ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2)    override;
  ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2)    override;

  ObjHandle   op_mux      (const ObjHandle& cond,
                            const ObjHandle& in1, const ObjHandle& in2)   override;

  /* clang-format on */

  void export_blif(std::ostream &stream,
                   const std::string &model_name = "CIRCUIT");

  void export_blif(const std::string &file_name,
                   const std::string &model_name = "CIRCUIT");

protected:
  obj_man::Basic<obj_man::Allocator<BTI::Node>> mm;

  ObjHandle add_gate(BTI::GateType gate_type,
                     const std::initializer_list<ObjHandleT<BTI::Node>> inps_p);
  ObjHandle add_input(const std::string &name = "");
  void make_output(const ObjHandleT<BTI::Node> &hdl,
                   const std::string &name = "");

  std::vector<ObjHandleT<BTI::Node>> inputs;
  std::vector<ObjHandleT<BTI::Node>> gates;
  std::vector<ObjHandleT<BTI::Node>> outputs;
};
} // namespace cingulata

#endif
