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
