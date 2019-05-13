
template <typename AllocT>
Basic<AllocT>::Basic(const AllocT &alloc)
    : m_alloc(alloc) {}

template <typename AllocT>
template <typename... Args>
ObjHandle Basic<AllocT>::new_handle(Args&& ... args) {
  return ObjHandle(m_alloc.new_obj(std::forward<Args>(args)...),
                   [this](void *ptr) { m_alloc.del_obj(ptr); });
}
