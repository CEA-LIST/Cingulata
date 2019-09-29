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

#ifndef CINGULATA_NODE
#define CINGULATA_NODE

#include <fmt/format.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace cingulata {

class Node {
public:
  typedef unsigned int id_t;

public:
  enum class NodeType : uint8_t { UNKNOWN = 0, INPUT, OUTPUT, GATE };
  enum class GateType : uint8_t {
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

public:
  Node(const id_t p_id) : m_id(p_id) {}

  bool is_input() const { return m_type == NodeType::INPUT; }

  bool is_output() const { return m_type == NodeType::OUTPUT; }

  bool is_gate() const { return m_type == NodeType::GATE; }

  NodeType get_type() const { return m_type; }
  GateType get_gate_type() const { return m_gate_type; }

  id_t get_id() const { return m_id; }

  const std::vector<id_t> &get_succs() const { return m_succs; }

  const std::vector<id_t> &get_preds() const { return m_preds; }

protected:
  void set_type(const NodeType p_type) { m_type = p_type; }
  void set_gate_type(const GateType p_gate_type) { m_gate_type = p_gate_type; }

private:
  id_t m_id;
  NodeType m_type = NodeType::UNKNOWN;
  GateType m_gate_type = GateType::UNKNOWN;

  std::vector<id_t> m_preds;
  std::vector<id_t> m_succs;

  friend class Circuit;
};

} // namespace cingulata

#include "node-fmt.hxx"

#endif
