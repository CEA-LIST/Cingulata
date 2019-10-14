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

#include <bit_exec/parallel/scheduler.hxx>

#include <bit_exec/parallel/worker.hxx>
#include <logging.hxx>

#include <thread>

using namespace std;
using namespace cingulata;

Scheduler::Scheduler(const std::vector<shared_ptr<IBitExec>> &p_bit_execs,
                     const size_t p_buffer_size) {
  for (size_t i = 0; i < p_bit_execs.size(); ++i) {
    m_workers.emplace_back(new Worker(this, p_bit_execs[i], i));
  }

  for (size_t i = 0; i < p_buffer_size; ++i) {
    m_slot_buffer.push_back(new Slot(Slot::EMPTY));
  }
}

void Scheduler::run(const Circuit &p_circuit,
                    unordered_map<string, ObjHandle> &outputs,
                    const unordered_map<string, ObjHandle> &inputs) {
  CINGU_LOG_DEBUG("{} Scheduler::run - start", this_thread::get_id());

  set_circuit(p_circuit);
  schedule_no_input_gates();
  set_inputs(inputs);

  vector<thread> threads;
  for (Worker *worker : m_workers) {
    // worker->run();
    threads.emplace_back(&Worker::run, worker);
  }

  for (thread &th : threads)
    th.join();

  CINGU_LOG_DEBUG("{} Scheduler::run - done", this_thread::get_id());
}

void Scheduler::set_circuit(const Circuit &p_circuit) {
  m_circuit = p_circuit;

  const size_t node_cnt = m_circuit.node_cnt();

  // clear previous data
  for (const auto *ptr : m_nb_exec_pred)
    delete ptr;
  m_nb_exec_pred.clear();

  // initially all predecessors are not executed
  for (size_t id = 0; id < node_cnt; ++id) {
    const size_t nb_preds = m_circuit.get_preds(id).size();
    m_nb_exec_pred.emplace_back(new atomic<int>(nb_preds));
  }

  m_handles.clear();
  m_handles.resize(node_cnt);

  // fill map with handles so that it'll be thread-safe
  for (const Node::id_t id : m_circuit.get_outputs())
    m_outputs[m_circuit.get_name(id)] = ObjHandle();

  m_nb_outputs_done.store(m_circuit.get_outputs().size());
}

void Scheduler::set_inputs(const unordered_map<string, ObjHandle> &inputs) {
  for (const Node::id_t id : m_circuit.get_inputs()) {
    const string &name = m_circuit.get_name(id);
    m_handles[id] = inputs.at(name);
    job_done(id);
  }
}

void Scheduler::schedule_no_input_gates() {
  for (const Node &node : m_circuit.get_nodes()) {
    if (node.get_preds().size() == 0 and node.is_gate()) {
      schedule_job(node.get_id());
    }
  }
}

void Scheduler::job_done(const Node::id_t id) {
  CINGU_LOG_DEBUG("{} Scheduler::job_done - {}", this_thread::get_id(),
               m_circuit.get_node(id));

  vector<Node::id_t> ready_nodes;

  const auto &succ_ids = m_circuit.get_succs(id);
  ready_nodes.reserve(succ_ids.size());

  // update executed predecessors count and find ready successor nodes (ie whose
  // predecessors are all executed)
  for (const Node::id_t sid : succ_ids) {
    auto &nb = *m_nb_exec_pred.at(sid); // TODO replace at with []
    nb--;

    int tmp = 0;
    bool avail_for_sched = nb.compare_exchange_strong(tmp, -1);
    if (avail_for_sched) {
      ready_nodes.emplace_back(sid);
    }
  }

  // push ready jobs to circular buffer empty slots
  for (const Node::id_t sid : ready_nodes) {
    schedule_job(sid);
  }
}

void Scheduler::schedule_job(const Node::id_t sid) {
  Node *node = &m_circuit.get_node(sid);

  if (node->is_output()) {
    assert(node->get_preds().size() == 1);
    const Node::id_t id = node->get_preds().front();
    m_outputs.at(m_circuit.get_name(sid)) = get_handle(id);
    m_nb_outputs_done--;
    CINGU_LOG_TRACE(
        "{} Scheduler::schedule_job - node {} output {} (remaining {})",
        this_thread::get_id(), *node, m_circuit.get_name(sid),
        m_nb_outputs_done);
  } else {
    Slot *slot = find_empty_slot();
    slot->node = node;
    slot->state = Slot::State::READY;
    CINGU_LOG_TRACE("{} Scheduler::schedule_job - node {}", this_thread::get_id(),
                 *node);
  }
}

Slot *Scheduler::find_empty_slot(const size_t start_idx) {
  CINGU_LOG_TRACE("{} Scheduler::find_empty_slot", this_thread::get_id());
  return find_slot(Slot::State::EMPTY, Slot::State::TAKEN, start_idx, false);
}

Slot *Scheduler::find_ready_slot(const size_t start_idx) {
  CINGU_LOG_TRACE("{} Scheduler::find_ready_slot", this_thread::get_id());
  return find_slot(Slot::State::READY, Slot::State::EXEC, start_idx, true);
}

Slot *Scheduler::find_slot(const Slot::State state, const Slot::State new_state,
                           const size_t start_idx,
                           const bool return_after_cycle) {
  size_t slot_idx = start_idx;
  Slot *slot = m_slot_buffer.at(slot_idx);
  Slot::State tmp = state;
  while (not slot->state.compare_exchange_weak(tmp, new_state)) {
    slot_idx = (slot_idx + 1) % m_slot_buffer.size();
    if (return_after_cycle and slot_idx == start_idx)
      return nullptr;

    slot = m_slot_buffer[slot_idx];
    tmp = state;
  }

  CINGU_LOG_TRACE("{} Scheduler::find_slot - found slot {}", this_thread::get_id(),
               slot_idx);
  return slot;
}
