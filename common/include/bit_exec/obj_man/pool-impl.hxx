/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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


template <typename AllocT>
Pool<AllocT>::Pool(const AllocT &alloc) : m_alloc(alloc) {}

template <typename AllocT> Pool<AllocT>::~Pool() {
  clear();
}

template <typename AllocT> void Pool<AllocT>::clear() {
  while (not m_alloc_obj.empty()) {
    void *ptr = m_alloc_obj.back();
    m_alloc_obj.pop_back();
    m_alloc.del_obj(ptr);
  }
}



template <typename AllocT>
template <typename... Args>
ObjHandle Pool<AllocT>::new_handle(Args&& ... args) {
  void *ptr = nullptr;
  if (m_alloc_obj.empty()) {
    ptr = m_alloc.new_obj(std::forward<Args>(args)...);
  } else {
    ptr = m_alloc_obj.back();
    m_alloc_obj.pop_back();
  }
  return ObjHandle(ptr, [this](void *ptr) { store_obj(ptr); });
}

template <typename AllocT> void Pool<AllocT>::store_obj(void *ptr) {
  m_alloc_obj.push_back(ptr);
}
