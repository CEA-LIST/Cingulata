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

#ifndef CINGULATA_PARALLEL_SCHEDULER
#define CINGULATA_PARALLEL_SCHEDULER

#include <bit_exec/circuit/circuit.hxx>
#include <bit_exec/interface.hxx>
#include <bit_exec/obj_handle.hxx>
#include <bit_exec/parallel/ring_buffer.hxx>

#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>

namespace cingulata {
namespace parallel {

class Worker;

class Scheduler {
  friend class Worker;

public:
  Scheduler(const std::vector<std::shared_ptr<IBitExec>> &p_bit_execs);

  ~Scheduler();

  void run(const Circuit &p_circuit,
           std::unordered_map<std::string, ObjHandle> &outputs,
           const std::unordered_map<std::string, ObjHandle> &inputs);

private:
  void set_circuit(const Circuit &p_circuit);
  void
  set_inputs(const std::unordered_map<std::string, ObjHandle> &inp_out_hdls);
  const std::unordered_map<std::string, ObjHandle> &get_outputs() const {
    return m_outputs;
  }
  void schedule_no_input_gates();

  bool is_finished() { return m_nb_outputs_done == 0; }

  void job_done(const Node *const node);
  void schedule_job(const Node *const node);
  const Node *get_next_job() { return m_ring_buffer.pop(); }

  ObjHandle get_handle(const Node::id_t id) const {
    CINGU_LOG_TRACE("{} Scheduler::get_handle - {}", std::this_thread::get_id(),
                    m_circuit.get_node(id));
    assert(id < m_handles.size());
    assert(not m_handles[id].is_empty());
    return m_handles[id];
  }

  void set_handle(const Node::id_t id, ObjHandle hdl) {
    CINGU_LOG_TRACE("{} Scheduler::set_handle - {}", std::this_thread::get_id(),
                    m_circuit.get_node(id));
    assert(id < m_handles.size());
    assert(m_handles[id].is_empty());
    m_handles[id] = hdl;
  }

  void del_handle(const Node::id_t id) {
    CINGU_LOG_TRACE("{} Scheduler::del_handle - {}", std::this_thread::get_id(),
                    m_circuit.get_node(id));
    assert(id < m_handles.size());
    assert(not m_handles[id].is_empty());
    m_handles[id].reset();
  }

  std::vector<Node::id_t> get_ready_succ_ids(const Node *const node);
  std::vector<Node::id_t> get_useless_pred_ids(const Node *const node);

private:
  Circuit m_circuit;

  std::vector<Worker *> m_workers;

  RingBuffer<const Node> m_ring_buffer;

  std::vector<std::atomic<int> *>
      m_nb_exec_pred; // if m_nb_exec_pred[v]==0 => v is available for
                      // execution, m_nb_exec_pred[v]==-1 => executed or in
                      // execution queue

  std::vector<std::atomic<int> *>
      m_nb_exec_succ; // if m_nb_exec_succ[v]==0 => v is no longer used so
                      // handle can be deleted, m_nb_exec_succ[v]==-1 =>
                      // handle already deleted

  std::vector<ObjHandle> m_handles;

  std::unordered_map<std::string, ObjHandle> m_outputs;
  std::atomic<size_t> m_nb_outputs_done;
};

} // namespace parallel
} // namespace cingulata

#endif
