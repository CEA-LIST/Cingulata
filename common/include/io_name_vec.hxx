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

#ifndef IO_NAME_VEC
#define IO_NAME_VEC

#include <string>
#include <cassert>

namespace cingulata
{
  /**
   * @brief      Input/output operations for types composed of individual
   *             elements (@c CiInt, @c CiBitVector)
   * @details    This generic class provides input/output, encryption/decryption
   *             and name management functions for types @c T composed of
   *             multiple elements. Type @c T should define @c size and @c
   *             operator[] functions.
   *
   * @tparam     T     Type composed of multiple elements
   */
  template<typename T>
  class IoNameVec {
  public:
    /**
     * @brief      Sets the format to use for constructing new names.
     * @details    A C-style format string for a @c printf like function should
     *             be used. The first format specifier must be a string (name)
     *             and the second one and integer (element index).
     *
     * @param[in]  p_name_fmt  The name format
     */
    static void set_format(const std::string& p_name_fmt) {
      m_name_fmt = p_name_fmt;
    }

  private:
    static std::string m_name_fmt;

  public:
    /**
     * @name Read/write, encrypt/decrypt methods of elements
     * @{
     */

    /**
     * @brief      Read elements using their names
     *
     * @return     reference to current object
     */
    T& read() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].read();
      }
      return ref;
    }

    /**
     * @brief      Read elements using their names
     *
     * @param[in]  p_name  The name
     *
     * @return     reference to current object
     */
    T& read(const std::string& p_name) {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].read(get_name(p_name, i));
      }
      return ref;
    }

    /**
     * @brief      Write elements using their names
     *
     * @return     reference to current object
     */
    const T& write() const {
      const T& ref = *static_cast<const T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].write();
      }
      return ref;
    }

    /**
     * @brief      Write elements using their names
     *
     * @param[in]  p_name  The name
     *
     * @return     reference to current object
     */
    const T& write(const std::string& p_name) const {
      const T& ref = *static_cast<const T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].write(get_name(p_name, i));
      }
      return ref;
    }

    /**
     * @brief      Encrypt all elements
     *
     * @return     reference to current object
     */
    T& encrypt() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].encrypt();
      }
      return ref;
    }

    /**
     * @brief      Decrypt all elements
     */
    T& decrypt() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].decrypt();
      }
      return ref;
    }

    /**
     * @brief      Gets bits values encoded into an uint64_t.
     *
     * @return     integer value
     */
    uint64_t get_val() const {
      return decode_plain_int<uint64_t>(get_bits_val(sizeof(uint64_t)*8));
    }

    /**
     * @brief      Gets bits values
     * @note       This method supposes that method @c CiBit::get_val return bit
     *             value (i.e. all bits are plain)
     *
     * @param[in]  p_bit_cnt  The number of bits to return. If value -1 (default
     *                        value) is given all bits are returned.
     *
     * @return     vector with bit values
     */
    std::vector<bit_plain_t> get_bits_val(const size_t p_bit_cnt = -1) const {
      const T& ref = *static_cast<const T*>(this);
      std::vector<bit_plain_t> bv(ref.size());
      for (int i = 0; i < ref.size(); ++i)
        bv[i] = ref[i].get_val();
      return bv;
    }

    /**
     * @}
     */

  protected:

    /**
     * @brief      Get name of an element according to format defined by @c
     *             m_name_fmt
     *
     * @param[in]  p_name   Common part
     * @param[in]  p_order  Element order
     *
     * @return     formatted name
     */
    static std::string get_name(const std::string& p_name, const unsigned p_order) {
      constexpr static int m_name_size_max = 256;
      static thread_local char buf[m_name_size_max];
      int n = std::snprintf(buf, m_name_size_max, m_name_fmt.c_str(), p_name.c_str(), p_order);
      assert(0 <= n && n < m_name_size_max);
      return buf;
    }
  };
} // cingulata

#endif // IO_NAME_VEC
