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

#include <string>
#include <vector>

namespace cingulata {

class Circuit {
public:
  typedef unsigned int node_id_t;

  class Node;
  enum class NodeType : uint8_t;
  enum class GateType : uint8_t;

public:

  Circuit() = default;

  void clear();

  const std::string &get_name() const { return m_name; }
  void set_name(const std::string &p_name) { m_name = p_name; }

  node_id_t add_input();
  node_id_t add_gate(const GateType p_gate_type,
                     const std::initializer_list<node_id_t> &p_preds);
  void make_output(const node_id_t p_id);

  size_t node_cnt() const { return m_nodes.size(); }

  const std::vector<Node> &get_nodes() const { return m_nodes; }
  std::vector<Node> &get_nodes() { return m_nodes; }

  Node &get_node(const node_id_t p_id) { return m_nodes.at(p_id); }
  const Node &get_node(const node_id_t p_id) const { return m_nodes.at(p_id); }

  const std::vector<node_id_t> &get_preds(const node_id_t p_id) const {
    return m_node_preds.at(p_id);
  }

  const std::vector<node_id_t> &get_succs(const node_id_t p_id) const {
    return m_node_succs.at(p_id);
  }

  const std::vector<node_id_t> &get_inputs() const { return m_input_ids; }
  const std::vector<node_id_t> &get_outputs() const { return m_output_ids; }

protected:
  node_id_t add_node(const std::initializer_list<node_id_t> &p_pred_ids = {});

private:
  std::string m_name;

  // m_nodes[id] - node "id" data object
  std::vector<Node> m_nodes;

  // list of predecessors per node
  std::vector<std::vector<node_id_t>> m_node_preds;

  // list of successors per node
  std::vector<std::vector<node_id_t>> m_node_succs;

  std::vector<node_id_t> m_input_ids;
  std::vector<node_id_t> m_output_ids;
};

enum class Circuit::NodeType : uint8_t {
  UNKNOWN = 0,
  INPUT = 1 << 0,
  OUTPUT = 1 << 1,
  GATE = 1 << 2
};

enum class Circuit::GateType : uint8_t {
  UNKNOWN = 0,
  ZERO,
  ONE,
  NOT,
  BUF,
  AND,
  NAND,
  ANDNY,
  ANDYN,
  OR,
  NOR,
  ORNY,
  ORYN,
  XOR,
  XNOR,
  MUX
};

class Circuit::Node {
public:
  Node() = default;

public:
  bool is_input() const {
    return static_cast<uint8_t>(m_type) & static_cast<uint8_t>(NodeType::INPUT);
  }

  bool is_output() const {
    return static_cast<uint8_t>(m_type) &
           static_cast<uint8_t>(NodeType::OUTPUT);
  }

  bool is_gate() const {
    return static_cast<uint8_t>(m_type) & static_cast<uint8_t>(NodeType::GATE);
  }

  const std::string &get_name() const { return m_name; }
  void set_name(const std::string &p_name) { m_name = p_name; }

  const NodeType type() const { return m_type; }
  const GateType gate_type() const { return m_gate_type; }

protected:
  NodeType &type() { return m_type; }
  GateType &gate_type() { return m_gate_type; }

  void make_output() {
    m_type = static_cast<NodeType>(static_cast<uint8_t>(m_type) |
                                   static_cast<uint8_t>(NodeType::OUTPUT));
  }

private:
  std::string m_name;
  NodeType m_type = NodeType::UNKNOWN;
  GateType m_gate_type = GateType::UNKNOWN;

  friend class Circuit;
};


} // namespace cingulata

#endif
