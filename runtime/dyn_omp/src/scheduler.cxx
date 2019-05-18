/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

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

#include "scheduler.hxx"

using namespace std;

Scheduler::Scheduler(const Circuit &circuit_p, Priority *const priority_p)
    : circuit(circuit_p), waitQueue(Scheduler::PriorityComparator(priority_p)) {

  initScheduler();
}

Scheduler::Operation Scheduler::next() {
  Scheduler::Operation oper;
  unique_lock<mutex> lck(waitQueueMtx);

  while (waitQueue.empty() and not schedFinished()) {
    waitQueueCond.wait(lck);
  }

  if (schedFinished()) {
    oper = Scheduler::Operation{Circuit::null_vertex(),
                                Scheduler::Operation::Type::Done};
  } else {
    oper = waitQueue.top();
    waitQueue.pop();
  }

  return oper;
}

void Scheduler::done(const Scheduler::Operation &oper) {
  unique_lock<mutex> lck(finishedQueueMtx);
  finishedQueue.push(oper);
  lck.unlock();
  finishedQueueCond.notify_one();
}

void Scheduler::doSchedule() {
  while (not schedFinished()) {
    Scheduler::Operation oper = popFinishedQueue();
    if (oper.type == Scheduler::Operation::Type::Execute) {
      executeOperFinished(oper);
    }
  }

  waitQueueCond.notify_all();
}

void Scheduler::initScheduler() {
  /* Input nodes (in_degree == 0) are available for execution directly */
  for (auto it = vertices(circuit); it.first != it.second; ++it.first) {
    const Circuit::vertex_descriptor &node = *(it.first);
    if (in_degree(node, circuit) == 0) {
      pushExecuteCmd(node);
    }
  }
}

Scheduler::Operation Scheduler::popFinishedQueue() {
  unique_lock<mutex> lck(finishedQueueMtx);
  finishedQueueCond.wait(lck, [this] { return !this->finishedQueue.empty(); });
  Scheduler::Operation oper = finishedQueue.front();
  finishedQueue.pop();
  return oper;
}

void Scheduler::pushWaitQueue(const Circuit::vertex_descriptor node,
                              const Scheduler::Operation::Type type) {
  lock_guard<mutex> lck(waitQueueMtx);
  waitQueue.push(Scheduler::Operation{node, type});
  waitQueueCond.notify_one();
}

void Scheduler::pushDeleteCmd(const Circuit::vertex_descriptor node) {
  pushWaitQueue(node, Scheduler::Operation::Type::Delete);
}

void Scheduler::pushExecuteCmd(const Circuit::vertex_descriptor node) {
  pushWaitQueue(node, Scheduler::Operation::Type::Execute);
}

void Scheduler::executeOperFinished(const Scheduler::Operation &oper) {
  if (out_degree(oper.node, circuit) > 0) {
    succ2ExecCnt.emplace(oper.node, out_degree(oper.node, circuit));
  } else {
    pushDeleteCmd(oper.node);
  }

  /* Update <successors to execute> counter for current node predecessors
      and push delete predecessor commands when needed */
  for (auto it = inv_adjacent_vertices(oper.node, circuit);
       it.first != it.second; ++it.first) {
    const Circuit::vertex_descriptor &pred = *(it.first);
    succ2ExecCnt[pred]--;
    if (succ2ExecCnt[pred] == 0) {
      pushDeleteCmd(pred);
    }
  }

  /* Update <predecessors to execute> counter for current node successors
      and push execute successor commands when needed */
  for (auto it = adjacent_vertices(oper.node, circuit); it.first != it.second;
       ++it.first) {
    const Circuit::vertex_descriptor &succ = *(it.first);
    if (in_degree(succ, circuit) == 1) {
      pushExecuteCmd(succ);
    } else {
      if (pred2ExecCnt.find(succ) == pred2ExecCnt.end()) {
        pred2ExecCnt.emplace(succ, in_degree(succ, circuit));
      }
      pred2ExecCnt[succ]--;
      if (pred2ExecCnt[succ] == 0) {
        pushExecuteCmd(succ);
      }
    }
  }
  executedCnt++;
}

bool Scheduler::schedFinished() { return executedCnt == num_vertices(circuit); }
