/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.

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

#include <bit_exec/circuit/circuit.hxx>

#include <cassert>

using namespace cingulata;

void Circuit::clear() {
  m_name.clear();
  m_nodes.clear();
  m_node_preds.clear();
  m_node_succs.clear();
  m_input_ids.clear();
  m_output_ids.clear();
}

Circuit::node_id_t Circuit::add_input() {
  node_id_t id = add_node();
  m_nodes[id].type() = NodeType::INPUT;
  m_input_ids.emplace_back(id);
  return id;
}

Circuit::node_id_t
Circuit::add_gate(const GateType p_gate_type,
                  std::initializer_list<node_id_t> p_pred_ids) {
  return add_gate(p_gate_type, p_pred_ids.begin(), p_pred_ids.end());
}

void Circuit::make_output(const node_id_t p_id) {
  m_nodes.at(p_id).make_output();
  m_output_ids.emplace_back(p_id);
}

Circuit::node_id_t
Circuit::add_node(std::initializer_list<node_id_t> p_pred_ids) {
  return add_node(p_pred_ids.begin(), p_pred_ids.end());
}
