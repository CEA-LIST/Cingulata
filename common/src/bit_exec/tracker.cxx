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

#include <bit_exec/circuit/blif_io.hxx>
#include <bit_exec/circuit/circuit.hxx>
#include <bit_exec/tracker.hxx>

#include <fstream>

using namespace std;
using namespace cingulata;

void BitTracker::reset() { m_circuit.clear(); }

ObjHandle BitTracker::encode(const bit_plain_t pt_val) {
  Circuit::node_id_t id;
  if (pt_val == 0) {
    id = m_circuit.add_gate(Circuit::GateType::ZERO, {});
  } else {
    id = m_circuit.add_gate(Circuit::GateType::ONE, {});
  }
  return mm.new_handle(id);
}

ObjHandle BitTracker::encrypt(const bit_plain_t pt_val) {
  return mm.new_handle(m_circuit.add_input());
}

bit_plain_t BitTracker::decrypt(const ObjHandle &hdl) {
  Circuit::node_id_t id = *hdl.get<Circuit::node_id_t>();
  m_circuit.add_output(id);
  return 0;
}

ObjHandle BitTracker::read(const string &name) {
  Circuit::node_id_t id = m_circuit.add_input(name);
  return mm.new_handle(id);
}

void BitTracker::write(const ObjHandle &hdl, const string &name) {
  Circuit::node_id_t id = *hdl.get<Circuit::node_id_t>();
  m_circuit.add_output(id, name);
}

#define DEFINE_1_INP_OPER(OP_NAME, GATE_TYPE)                                  \
  ObjHandle BitTracker::OP_NAME(const ObjHandle &lhs) {                        \
    Circuit::node_id_t lhs_id = *lhs.get<Circuit::node_id_t>();                \
    Circuit::node_id_t id =                                                    \
        m_circuit.add_gate(Circuit::GateType::GATE_TYPE, {lhs_id});            \
    return mm.new_handle(id);                                                  \
  }

#define DEFINE_2_INP_OPER(OP_NAME, GATE_TYPE)                                  \
  ObjHandle BitTracker::OP_NAME(const ObjHandle &lhs, const ObjHandle &rhs) {  \
    Circuit::node_id_t lhs_id = *lhs.get<Circuit::node_id_t>();                \
    Circuit::node_id_t rhs_id = *rhs.get<Circuit::node_id_t>();                \
    Circuit::node_id_t id =                                                    \
        m_circuit.add_gate(Circuit::GateType::GATE_TYPE, {lhs_id, rhs_id});    \
    return mm.new_handle(id);                                                  \
  }

// DEFINE_1_INP_OPER(op_not, NOT);
DEFINE_2_INP_OPER(op_and, AND);
DEFINE_2_INP_OPER(op_xor, XOR);
// DEFINE_2_INP_OPER(op_nand, NAND);
// DEFINE_2_INP_OPER(op_andyn, ANDYN);
// DEFINE_2_INP_OPER(op_andny, ANDNY);
// DEFINE_2_INP_OPER(op_or, OR);
// DEFINE_2_INP_OPER(op_nor, NOR);
// DEFINE_2_INP_OPER(op_oryn, ORYN);
// DEFINE_2_INP_OPER(op_orny, ORNY);
// DEFINE_2_INP_OPER(op_xnor, XNOR);

// ObjHandle BitTracker::op_mux(const ObjHandle& cond, const ObjHandle& in1,
// const ObjHandle& in2) {
//   return m_circuit.add_gate(Circuit::GateType::MUX, {cond, in1, in2});
// }

void BitTracker::export_blif(ostream &stream, const string &model_name) {
  m_circuit.set_name(model_name);
  BlifOutput().write(stream, m_circuit);
}

void BitTracker::export_blif(const string &file_name,
                             const string &model_name) {
  m_circuit.set_name(model_name);
  BlifOutput().write(file_name, m_circuit);
}
