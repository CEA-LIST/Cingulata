#ifndef MEM_POOL
#define MEM_POOL

#include <bit_exec/obj_handle.hxx>
#include <bit_exec/obj_man/allocator.hxx>

#include <deque>

namespace cingulata {
namespace obj_man {

/**
 * @brief      An object pool management class
 * @details    Objects are not deleted, but instead pushed to a pool of objects.
 *             Each time a new object handler is requested either a new object
 *             is created or an existing one (created and returned to object
 *             pool earlier) is returned.
 *
 * @tparam     AllocT  Allocator type
 */
template <typename AllocT> class Pool {
public:
  /**
   * @brief      Constructs the object pool memory management object
   *
   * @param[in]  alloc  The allocator to use
   */
  Pool(const AllocT &alloc = AllocT());

  /**
   * @brief      Destroys pool and its objects
   */
  ~Pool();

  /**
   * @brief      Create a generic object handle
   *
   * @param[in]  args  parameters to pass to @c AllocT object create method.
   *
   * @tparam     Args  parameter pack
   *
   * @return     a new generic object handle
   */
  template <typename... Args> ObjHandle new_handle(Args&& ... args);

  /**
   * @brief      Destroys all objects in pool
   */
  void clear();

protected:
  void store_obj(void *ptr);

  std::deque<void *> m_alloc_obj;
  const AllocT &m_alloc;
};

#include "pool-impl.hxx"

} // namespace obj_man
} // namespace cingulata

#endif
