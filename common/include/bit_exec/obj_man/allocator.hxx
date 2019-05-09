#ifndef ALLOCATOR
#define ALLOCATOR

namespace cingulata {
namespace obj_man {

/**
 * @brief      Generic allocator class for type @c ObjT objects
 *
 * @tparam     ObjT  type of objects to allocate
 */
template <typename ObjT> class Allocator {
public:
  /**
   * @brief      Create a new object
   *
   * @param[in]  args  The arguments to pass to object constructor
   *
   * @tparam     Args  parameter pack
   *
   * @return     pointer to new object
   */
  template <typename... Args> void *new_obj(Args... args) const {
    return new ObjT(std::forward<args>...);
  }

  /**
   * @brief      Delete an object
   *
   * @param      ptr   pointer to object to delete
   */
  void del_obj(void *ptr) const { delete static_cast<ObjT *>(ptr); }
};

} // namespace obj_man
} // namespace cingulata

#endif
