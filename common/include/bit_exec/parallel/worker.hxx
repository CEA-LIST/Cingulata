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

#ifndef CINGULATA_PARALLEL_WORKER
#define CINGULATA_PARALLEL_WORKER

#include <bit_exec/interface.hxx>

#include <cassert>

namespace cingulata {

class Scheduler;
class Slot;

class Worker {
public:
  Worker(Scheduler *p_scheduler, IBitExec *const p_bit_exec, int p_id)
      : m_scheduler(p_scheduler), m_bit_exec(p_bit_exec), m_id(p_id) {
    assert(p_scheduler != nullptr);
    assert(m_bit_exec != nullptr);
  }

  void run();

private:
  void exec(const Slot *const slot);

  Scheduler *m_scheduler;
  IBitExec *m_bit_exec;
  int m_id;
};

} // namespace cingulata

#endif
