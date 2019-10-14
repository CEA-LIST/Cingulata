template <typename InputIterator>
Node::id_t Circuit::add_node(const InputIterator first,
                                     const InputIterator last) {
  Node::id_t nid = m_nodes.size();
  m_nodes.emplace_back(nid);
  Node &node = m_nodes.back();

  // add node as successor to each predecessor
  for (InputIterator it = first; it != last; ++it) {
    const Node::id_t &id = *it;

    node.m_preds.emplace_back(id);
    get_node(id).m_succs.emplace_back(nid);
  }

  return nid;
}

template <typename InputIterator>
Node::id_t Circuit::add_gate(const Node::GateType p_gate_type,
                                     const InputIterator first,
                                     const InputIterator last) {
  Node::id_t id = add_node(first, last);

  Node &node = m_nodes[id];
  node.set_type(Node::NodeType::GATE);
  node.set_gate_type(p_gate_type);

  CINGU_LOG_DEBUG("Circuit::add_gate - {}", node);

  return id;
}
