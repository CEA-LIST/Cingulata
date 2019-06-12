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

#ifndef BIT_EXEC_OBJ_HANDLE
#define BIT_EXEC_OBJ_HANDLE

#include <memory>

namespace cingulata
{
  /**
   * Typed object handle, implemented as a shared pointer
   */
  template<typename T>
  using ObjHandleT = std::shared_ptr<T>;

  /**
   * @brief Generic object handle
   * @details Implementation based on a shared pointer to void
   */
  class ObjHandle : public ObjHandleT<void>
  {
  public:
    /**
     * @brief Construct an empty handle
     */
    ObjHandle() : ObjHandleT<void>() {}

    /**
     * @brief Construct a handle for an existing typed object
     *
     * @param ptr pointer to an object
     * @param d deleter functionality
     * @tparam T object type
     * @tparam _Del deleter type
     */
    template<typename T, typename _Del>
    ObjHandle(T* ptr, _Del d) : std::shared_ptr<void>(ptr, d) {}

    /**
     * @brief Copy-construct a generic handle from a typed object handle
     *
     * @param hdl typed handle
     * @tparam T type of object handle
     */
    template<typename T>
    ObjHandle(const ObjHandleT<T>& hdl) : ObjHandleT<void>(hdl) {}

    /**
     * @brief Verify if object handle points to an object
     */
    bool is_empty() const {
      return not (bool)*this;
    }

    /**
     * @brief Cast operator to typed object handle
     *
     * @return a typed object handle
     */
    template<typename T>
    operator ObjHandleT<T> () const {
      return std::static_pointer_cast<T>(*this);
    }

    /**
     * @brief      Return stored pointer casted to type T
     *
     * @tparam     T     type of pointer to get
     *
     * @return     pointer of type T
     */
    template<typename T>
    T* get() const {
      return ((ObjHandleT<T>)*this).get();
    }
  };
}

#endif
