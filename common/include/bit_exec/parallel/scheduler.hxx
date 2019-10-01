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

#include <atomic>
#include <unordered_map>
#include <vector>

namespace cingulata {

class Worker;

class Slot {
public:
  enum State : uint8_t;
  std::atomic<State> state;

  Node *node;

  Slot(State p_state) : state(p_state) {}
};

enum Slot::State : uint8_t {
  EMPTY, // empty
  TAKEN, // reserved
  READY, // ready for execution
  EXEC   // in execution
};

class Scheduler {
public:
  Scheduler(const std::vector<IBitExec *> &p_bit_execs,
            const size_t p_buffer_size);

  void run(const Circuit &p_circuit,
           std::unordered_map<std::string, ObjHandle> &outputs,
           const std::unordered_map<std::string, ObjHandle> &inputs);

  void job_done(const Node::id_t id);

  void schedule_job(const Node::id_t id);

  bool is_finished() { return m_nb_outputs_done == 0; }

  Slot *find_empty_slot(const size_t start_idx = 0);
  Slot *find_ready_slot(const size_t start_idx = 0);
  Slot *find_slot(const Slot::State state, const Slot::State new_state,
                  const size_t start_idx);

  ObjHandle get_handle(const Node::id_t id) const {
    return m_handles.at(id); // TODO replace with []
  }

  void set_handle(const Node::id_t id, ObjHandle hdl) {
    m_handles.at(id) = hdl; // TODO replace with []
  }

  const Circuit &get_circuit() const { return m_circuit; }

private:
  void set_circuit(const Circuit &p_circuit);
  void
  set_inputs(const std::unordered_map<std::string, ObjHandle> &inp_out_hdls);
  void schedule_no_input_gates();
  const std::unordered_map<std::string, ObjHandle> &get_outputs() const {
    return m_outputs;
  }

  Circuit m_circuit;

  std::vector<Worker *> m_workers;

  std::vector<Slot *> m_slot_buffer;

  std::vector<std::atomic<int> *>
      m_nb_exec_pred; // if m_nb_exec_pred[v]==0 => v is available for
                      // execution, m_nb_exec_pred[v]==-1 => executed or in
                      // execution queue

  std::vector<ObjHandle> m_handles;

  std::unordered_map<std::string, ObjHandle> m_outputs;
  std::atomic<size_t> m_nb_outputs_done;
};

} // namespace cingulata

#endif
