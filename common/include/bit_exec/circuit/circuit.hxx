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

#ifndef CINGULATA_CIRCUIT
#define CINGULATA_CIRCUIT

#include "node.hxx"

#include <spdlog/spdlog.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace cingulata {

class Circuit {
public:
  Circuit() = default;

  void clear();

  const std::string &get_name() const { return m_name; }
  void set_name(const std::string &p_name) { m_name = p_name; }

  Node::id_t add_input(const std::string &p_name = "");
  Node::id_t add_gate(const Node::GateType p_gate_type,
                     std::initializer_list<Node::id_t> p_preds);
  template <typename InputIterator>
  Node::id_t add_gate(const Node::GateType p_gate_type, const InputIterator begin,
                     const InputIterator end);

  Node::id_t add_output(const Node::id_t p_id, const std::string &p_name = "");

  size_t node_cnt() const { return m_nodes.size(); }

  const std::vector<Node> &get_nodes() const { return m_nodes; }
  std::vector<Node> &get_nodes() { return m_nodes; }

  Node &get_node(const Node::id_t p_id) { return m_nodes.at(p_id); }
  const Node &get_node(const Node::id_t p_id) const { return m_nodes.at(p_id); }

  const std::vector<Node::id_t> &get_preds(const Node::id_t p_id) const {
    return m_nodes.at(p_id).get_preds();
  }

  const std::vector<Node::id_t> &get_succs(const Node::id_t p_id) const {
    return m_nodes.at(p_id).get_succs();
  }

  const std::vector<Node::id_t> &get_inputs() const { return m_input_ids; }
  const std::vector<Node::id_t> &get_outputs() const { return m_output_ids; }

  const std::string &get_name(const Node::id_t id) const {
    return m_io_names.at(id);
  }

  Node::id_t add_node(std::initializer_list<Node::id_t> p_pred_ids = {});

  template <typename InputIterator>
  Node::id_t add_node(const InputIterator begin, const InputIterator end);

private:
  std::string m_name;

  // m_nodes[id] - node "id" data object
  std::vector<Node> m_nodes;

  std::vector<Node::id_t> m_input_ids;
  std::vector<Node::id_t> m_output_ids;

  std::unordered_map<Node::id_t, std::string> m_io_names;
};

#include "circuit-impl.hxx"

} // namespace cingulata

#endif
