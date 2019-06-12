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

#ifndef MEM_MAN_BASIC
#define MEM_MAN_BASIC

#include <bit_exec/obj_handle.hxx>
#include <bit_exec/obj_man/allocator.hxx>

namespace cingulata {
namespace obj_man {

/**
 * @brief      A basic memory management class
 * @details    Each time a new object handler is requested a new object is
 *             created and is deletes when no object point to it.
 *
 * @tparam     AllocT  Allocator type
 */
template <typename AllocT> class Basic {
public:
  /**
   * @brief      Constructs the basic memory management object
   *
   * @param[in]  alloc  The allocator to use
   */
  Basic(const AllocT &alloc = AllocT());

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
  template <typename... Args> ObjHandle new_handle(Args... args);

protected:
  const AllocT &m_alloc;
};

#include "basic-impl.hxx"

} // namespace obj_man
} // namespace cingulata

#endif
