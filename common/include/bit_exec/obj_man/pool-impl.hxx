
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
ObjHandle Pool<AllocT>::new_handle(Args... args) {
  void *ptr = nullptr;
  if (m_alloc_obj.empty()) {
    ptr = m_alloc.new_obj(std::forward<args>...);
  } else {
    ptr = m_alloc_obj.back();
    m_alloc_obj.pop_back();
  }
  return ObjHandle(ptr, [this](void *ptr) { store_obj(ptr); });
}

template <typename AllocT> void Pool<AllocT>::store_obj(void *ptr) {
  m_alloc_obj.push_back(ptr);
}
