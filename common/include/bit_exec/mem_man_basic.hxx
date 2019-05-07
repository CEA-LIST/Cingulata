#ifndef MEM_MAN_BASIC
#define MEM_MAN_BASIC

#include <bit_exec/allocator.hxx>
#include <bit_exec/obj_handle.hxx>

namespace cingulata {

/**
 * @brief      A basic memory management class
 * @details    Each time a new object handler is requested a new object is
 *             created and is deletes when no object point to it.
 *
 * @tparam     AllocT  Allocator type
 */
template <typename AllocT> class MemManBasic {
public:
  /**
   * @brief      Constructs the basic memory management object
   *
   * @param[in]  alloc  The allocator to use
   */
  MemManBasic(const AllocT &alloc = AllocT()) : m_alloc(alloc) {}

  /**
   * @brief      Create a generic object handle
   * @details    Use method @c AllocT::new_obj to create a new object for the
   *             generic object handle and method @c AllocT::del_obj to delete
   *             this object when it is not in use anymore.
   *
   * @param[in]  args  parameters to pass to @c AllocT object create method.
   *
   * @tparam     Args  parameter pack
   *
   * @return     a new generic object handle
   */
  template <typename... Args> ObjHandle new_handle(Args... args) {
    return ObjHandle(m_alloc.new_obj(std::forward<args>...),
                     [this](void *ptr) { m_alloc.del_obj(ptr); });
  }

protected:
  const AllocT &m_alloc;
};

} // namespace cingulata

#endif
