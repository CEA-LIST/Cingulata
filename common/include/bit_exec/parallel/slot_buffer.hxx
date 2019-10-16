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

#ifndef CINGULATA_PARALLEL_SLOT_BUFFER
#define CINGULATA_PARALLEL_SLOT_BUFFER

#include <atomic>

namespace cingulata {
namespace parallel {

template<typename T>
class Buffer {
private:
  class Slot {
  public:
    enum State : uint8_t {
      EMPTY, // empty
      TAKEN, // reserved
      READY  // ready for execution
    };

    Slot() : data(nullptr), state(Slot::State::EMPTY), next(nullptr) {}

    T *data;
    std::atomic<Slot::State> state;
    Slot *next;
  };

public:
  Buffer(const std::size_t p_buffer_size) {
    m_head_ptr = new Slot[p_buffer_size];

    m_head_ptr[p_buffer_size - 1].next = m_head_ptr;
    for (std::size_t i = 0; i < p_buffer_size - 1; ++i) {
      m_head_ptr[i].next = m_head_ptr + (i + 1);
    }
  }

  ~Buffer() { delete[] m_head_ptr; }

  /**
   * @brief      Pushes data to an empty location of the buffer. Calling thread
   * is blocked untill an empty is found.
   *
   * @param      data  The data
   */
  void push(T *const data) {
    Slot *slot = find_slot(Slot::State::EMPTY, m_head_ptr, false);
    assert(slot != nullptr);
    slot->data = data;
    slot->state = Slot::State::READY;
  }

  /**
   * @brief      Pops data from the buffer
   *
   * @return     data from a ready slot or nullptr if none is available
   */
  T *pop() {
    Slot *slot = find_slot(Slot::State::READY, m_head_ptr, true);
    if (slot == nullptr)
      return nullptr;
    T *data = slot->data;
    slot->state = Slot::State::EMPTY;
    return data;
  }

private:
  Slot *find_slot(const typename Slot::State state, Slot *const start_slot,
                  const bool return_after_cycle) {

    const Slot *const end_slot = return_after_cycle ? start_slot : nullptr;
    Slot *slot = start_slot;

    typename Slot::State tmp = state;
    while (not slot->state.compare_exchange_weak(tmp, Slot::State::TAKEN)) {
      slot = slot->next;
      assert(slot != nullptr);

      if (slot == end_slot)
        return nullptr;
      tmp = state;
    }

    return slot;
  }

  Slot *m_head_ptr;
};

} // namespace parallel
} // namespace cingulata

#endif
