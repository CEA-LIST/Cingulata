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

#ifndef CINGULATA_PARALLEL_RING_BUFFER
#define CINGULATA_PARALLEL_RING_BUFFER

#include <logging.hxx>

#include <atomic>
#include <mutex>
#include <thread>

namespace cingulata {
namespace parallel {

template <typename T> class RingBuffer {
private:
  static constexpr std::size_t START_SIZE = 128;

  class Slot {
  public:
    enum State : uint8_t {
      EMPTY, // empty
      TAKEN, // reserved
      READY  // ready
    };

    Slot() : state(Slot::State::EMPTY), next(nullptr), data(nullptr) {}

    std::atomic<Slot::State> state;
    Slot *next;
    T *data;
  };

public:
  RingBuffer() : m_first_slot(alloc_slots()) {
    m_last_slot = m_first_slot + (START_SIZE - 1);
    m_last_slot->next = m_first_slot;
    m_push_slot_ind = m_pop_slot_ind = m_first_slot;
  }

  ~RingBuffer() {
    Slot *tmp = m_first_slot;

    do {
      Slot *next = tmp[START_SIZE - 1].next;
      delete[] tmp;
      tmp = next;
    } while (m_first_slot != tmp);
  }

  /**
   * @brief      Pushes data to an empty location of the buffer. RingBuffer size
   * is incresed if no empty location is found.
   *
   * @param      data  The data
   */
  void push(T *const data) {
    Slot *slot = nullptr;
    while ((slot = find_slot(Slot::State::EMPTY, m_push_slot_ind)) == nullptr) {
      increase_size();
    }

    CINGU_LOG_DEBUG("{} RingBuffer<T>::push - data {} to slot {}",
                    std::this_thread::get_id(), (void *)data, (void *)slot);
    slot->data = data;
    slot->state = Slot::State::READY;
  }

  /**
   * @brief      Pops data from the buffer
   *
   * @return     data from a ready slot or nullptr if none is available
   */
  T *pop() {
    Slot *slot = find_slot(Slot::State::READY, m_pop_slot_ind);
    if (slot == nullptr)
      return nullptr;
    T *data = slot->data;
    slot->state = Slot::State::EMPTY;
    CINGU_LOG_DEBUG("{} RingBuffer<T>::pop - data {} from slot {}",
                    std::this_thread::get_id(), (void *)data, (void *)slot);
    return data;
  }

private:
  void increase_size() {
    CINGU_LOG_DEBUG("{} RingBuffer<T>::increase_size",
                    std::this_thread::get_id());

    Slot *tmp = alloc_slots();
    tmp[START_SIZE - 1].next = m_first_slot;

    // following instructions are critical
    std::lock_guard<std::mutex> lock(m_increase_size_mtx);
    m_last_slot->next = tmp;
    m_last_slot = tmp + START_SIZE - 1;
  }

  Slot *alloc_slots() {
    CINGU_LOG_DEBUG("{} RingBuffer<T>::alloc_slots",
                    std::this_thread::get_id());

    Slot *tmp = new Slot[START_SIZE];

    // link together elements
    for (std::size_t i = 0; i < START_SIZE - 1; ++i) {
      tmp[i].next = tmp + (i + 1);
    }

    return tmp;
  }

  Slot *find_slot(const typename Slot::State state, Slot *&start_slot) {
    assert(start_slot != nullptr);
    Slot *slot = start_slot;

    typename Slot::State tmp = state;
    while (not slot->state.compare_exchange_weak(tmp, Slot::State::TAKEN)) {
      slot = slot->next;
      assert(slot != nullptr);

      if (slot == start_slot)
        return nullptr;

      tmp = state;
    }

    start_slot = slot->next;
    return slot;
  }

  Slot *const m_first_slot;
  Slot *m_last_slot;
  Slot *m_push_slot_ind;
  Slot *m_pop_slot_ind;

  std::mutex m_increase_size_mtx;
};

} // namespace parallel
} // namespace cingulata

#endif
