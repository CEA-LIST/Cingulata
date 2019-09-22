template <typename InputIterator>
Circuit::node_id_t Circuit::add_node(const InputIterator first,
                                     const InputIterator last) {
  node_id_t nid = m_nodes.size();
  m_nodes.emplace_back();

  // fill predecessors and successors list
  m_node_succs.emplace_back();
  m_node_preds.emplace_back();

  // add node as successor to each predecessor
  for (InputIterator it = first; it != last; ++it) {
    const node_id_t &id = *it;
    m_node_preds.back().emplace_back(id);
    m_node_succs.at(id).emplace_back(nid);
  }

  return nid;
}

template <typename InputIterator>
Circuit::node_id_t Circuit::add_gate(const GateType p_gate_type,
                                     const InputIterator first,
                                     const InputIterator last) {
  node_id_t id = add_node(first, last);
  m_nodes[id].type() = NodeType::GATE;
  m_nodes[id].gate_type() = p_gate_type;
  return id;
}
