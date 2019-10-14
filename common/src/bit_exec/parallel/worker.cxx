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

#include <bit_exec/parallel/worker.hxx>

#include <bit_exec/parallel/scheduler.hxx>
#include <logging.hxx>

#include <thread>

using namespace std;
using namespace cingulata;

void Worker::run() {
  CINGU_LOG_DEBUG("{} Worker::run - start", this_thread::get_id());
  while (not m_scheduler->is_finished()) {
    Slot *slot = m_scheduler->find_ready_slot();
    if (slot == nullptr)
      continue;
    exec(slot);
    m_scheduler->job_done(slot->node->get_id());
    slot->state = Slot::EMPTY;
  }
  CINGU_LOG_DEBUG("{} Worker::run - done", this_thread::get_id());
}

void Worker::exec(const Slot *const slot) {
  const Node &node = *(slot->node);
  CINGU_LOG_DEBUG("{} Worker::exec - begin {}", this_thread::get_id(), node);

  assert(m_scheduler->get_handle(node.get_id()).is_empty());

  vector<ObjHandle> inp_hdls(node.get_preds().size());
  for (size_t i = 0; i < node.get_preds().size(); ++i) {
    const auto id = node.get_preds()[i];
    inp_hdls[i] = m_scheduler->get_handle(id);
    assert(not inp_hdls[i].is_empty());
  }

  ObjHandle out_hdl;
  switch (slot->node->get_gate_type()) {
  case Node::GateType::ZERO:
    out_hdl = m_bit_exec->encode(0);
    break;
  case Node::GateType::ONE:
    out_hdl = m_bit_exec->encode(1);
    break;
  case Node::GateType::NOT:
    assert(inp_hdls.size() == 1);
    out_hdl = m_bit_exec->op_not(inp_hdls[0]);
    break;
  case Node::GateType::BUF:
    assert(inp_hdls.size() == 1);
    out_hdl = inp_hdls[0];
    break;
  case Node::GateType::AND:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_and(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::NAND:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_nand(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::ANDNY:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_andny(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::ANDYN:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_andyn(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::OR:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_or(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::NOR:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_nor(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::ORNY:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_orny(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::ORYN:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_oryn(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::XOR:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_xor(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::XNOR:
    assert(inp_hdls.size() == 2);
    out_hdl = m_bit_exec->op_xnor(inp_hdls[0], inp_hdls[1]);
    break;
  case Node::GateType::MUX:
    assert(inp_hdls.size() == 3);
    out_hdl = m_bit_exec->op_mux(inp_hdls[0], inp_hdls[1], inp_hdls[2]);
    break;
  default:
    CINGU_LOG_WARN("{} Worker::exec - unknown operation (output handle is empty)",
                this_thread::get_id());
  }

  m_scheduler->set_handle(node.get_id(), out_hdl);
}
