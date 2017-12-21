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

/**
 * @file scheduler.hxx
 * @brief scheduler class
 */

#ifndef __SCHEDULER_HXX__
#define __SCHEDULER_HXX__

#include "blif_circuit.hxx"
#include "priority.hxx"

#include <queue>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <boost/graph/adjacency_list.hpp>

class Scheduler {
  public:
    /**
     * @brief Scheduler base operation
     */
    struct Operation {
      Circuit::vertex_descriptor node;
      enum class Type {
        Execute,
        Delete,
        Done
      } type;
    };
    
  private:
    /** 
     * @brief Class used for comparing task priorities in the wait queue
     */
    class PriorityComparator {
      private:
        Priority* priority = nullptr;
        
      public:
        PriorityComparator(Priority* const priority_p): priority(priority_p) {}

        bool operator()(const Operation& x, const Operation& y) {
          return priority->value(x.node) < priority->value(y.node);
        }
    };
    
  private:
    Circuit circuit;

    /* Number of gate successors/predecessors remaining to execute */
    std::unordered_map<Circuit::vertex_descriptor, int> succ2ExecCnt;
    std::unordered_map<Circuit::vertex_descriptor, int> pred2ExecCnt;    

    /* Queue of finished schedule operations and synchronization variables */
    std::queue<Operation> finishedQueue;
    std::mutex finishedQueueMtx;
    std::condition_variable finishedQueueCond;

    /* Priority queue for available schedule operations and synchronization variables */
    std::priority_queue<Operation, std::vector<Operation>, PriorityComparator> waitQueue;
    std::mutex waitQueueMtx;
    std::condition_variable waitQueueCond;
    
    unsigned int executedCnt = 0;

  public:
    /**
     * @brief Initialize scheduler object
     */
    Scheduler(const Circuit& circuit, Priority* const priority_p);

    /** @brief Get next operation to schedule
     */
    Operation next();

    /** @brief Notify operation finished
     */
    void done(const Operation& oper);

    /** @brief Execute scheduling loop
     */
    void doSchedule();

  private:

    /**
     * @brief Initialize scheduler internal state variables
     */    
    void initScheduler();

    /**
     * @brief Gets next finished operation
     */
    Operation popFinishedQueue();

    /**
     * @brief Push a schedule operation corresponding to \c node to the wait queue
     */
    void pushWaitQueue(const Circuit::vertex_descriptor node, const Operation::Type type);

    /**
     * @brief Specialization of \c pushWaitQueue for delete operations
     */
    void pushDeleteCmd(const Circuit::vertex_descriptor node);
    
    /**
     * @brief Specialization of \c pushWaitQueue for gate execute operations
     */
    void pushExecuteCmd(const Circuit::vertex_descriptor node);

    /**
     * @brief Executes finishing schedule operations (delete memory, etc.)
     */
    void executeOperFinished(const Operation& oper);

    /**
     * @brief returns true when all gate execute operations are done
     */
    bool schedFinished() ;
};

#endif
