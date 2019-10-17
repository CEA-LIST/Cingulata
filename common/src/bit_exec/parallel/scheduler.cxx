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

using namespace std;
using namespace cingulata;
using namespace cingulata::parallel;

Scheduler::Scheduler(const std::vector<shared_ptr<IBitExec>> &p_bit_execs) {
  for (size_t i = 0; i < p_bit_execs.size(); ++i) {
    m_workers.emplace_back(new Worker(this, p_bit_execs[i]));
  }
}

Scheduler::~Scheduler() {
  for (Worker *worker : m_workers)
    delete worker;
  for (const auto *ptr : m_nb_exec_pred)
    delete ptr;
  for (const auto *ptr : m_nb_exec_succ)
    delete ptr;
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
  CINGU_LOG_DEBUG("{} Scheduler::set_circuit - number of nodes {}",
                  this_thread::get_id(), p_circuit.node_cnt());

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

  // clear previous data
  for (const auto *ptr : m_nb_exec_succ)
    delete ptr;
  m_nb_exec_succ.clear();

  // initially all successors are needed
  for (size_t id = 0; id < node_cnt; ++id) {
    const size_t nb_succs = m_circuit.get_succs(id).size();
    m_nb_exec_succ.emplace_back(new atomic<int>(nb_succs));
  }

  m_handles.clear();
  m_handles.resize(node_cnt);

  // fill map with handles so that it'll be thread-safe
  for (const Node::id_t id : m_circuit.get_outputs())
    m_outputs[m_circuit.get_name(id)] = ObjHandle();

  m_nb_outputs_done.store(m_circuit.get_outputs().size());
}

void Scheduler::set_inputs(const unordered_map<string, ObjHandle> &inputs) {
  CINGU_LOG_DEBUG("{} Scheduler::set_inputs - {}", this_thread::get_id(),
                  inputs.size());

  for (const Node::id_t id : m_circuit.get_inputs()) {
    const string &name = m_circuit.get_name(id);
    assert(inputs.find(name) != inputs.end());
    m_handles[id] = inputs.at(name);
    job_done(id);
  }
}

void Scheduler::schedule_no_input_gates() {
  CINGU_LOG_DEBUG("{} Scheduler::schedule_no_input_gates",
                  this_thread::get_id());

  for (const Node &node : m_circuit.get_nodes()) {
    if (node.get_preds().size() == 0 and node.is_gate()) {
      schedule_job(node.get_id());
    }
  }
}

vector<Node::id_t> Scheduler::get_ready_succ_ids(const Node::id_t id) {
  vector<Node::id_t> nodes;

  const auto &succ_ids = m_circuit.get_succs(id);
  nodes.reserve(succ_ids.size());

  // update executed predecessors count and find ready successor nodes (ie whose
  // predecessors are all executed)
  for (const Node::id_t sid : succ_ids) {
    auto &nb = *m_nb_exec_pred.at(sid); // TODO replace at with []
    nb--;

    int tmp = 0;
    if (nb.compare_exchange_strong(tmp, -1)) {
      nodes.emplace_back(sid);
    }
  }

  return nodes;
}

vector<Node::id_t> Scheduler::get_useless_pred_ids(const Node::id_t id) {
  vector<Node::id_t> nodes;

  const auto &pred_ids = m_circuit.get_preds(id);
  nodes.reserve(pred_ids.size());

  // update executed predecessors count and find ready successor nodes (ie whose
  // predecessors are all executed)
  for (const Node::id_t pid : pred_ids) {
    auto &nb = *m_nb_exec_succ.at(pid); // TODO replace at with []
    nb--;

    int tmp = 0;
    if (nb.compare_exchange_strong(tmp, -1)) {
      nodes.emplace_back(pid);
    }
  }

  return nodes;
}

void Scheduler::job_done(const Node::id_t id) {
  CINGU_LOG_DEBUG("{} Scheduler::job_done - {}", this_thread::get_id(),
                  m_circuit.get_node(id));

  // push ready jobs to circular waiting list
  vector<Node::id_t> ready_nodes = get_ready_succ_ids(id);
  for (const Node::id_t sid : ready_nodes) {
    schedule_job(sid);
  }

  // clear handles for not needed nodes
  vector<Node::id_t> useless_nodes = get_useless_pred_ids(id);
  for (const Node::id_t pid : useless_nodes) {
    del_handle(pid);
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
    m_ring_buffer.push(node);
    CINGU_LOG_TRACE("{} Scheduler::schedule_job - node {}",
                    this_thread::get_id(), *node);
  }
}
