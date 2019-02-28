#ifndef IO_NAME_VEC
#define IO_NAME_VEC

#include <string>

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
     * @name Name management functions
     * @{
     */

    /**
     * @brief      Set name of @c T elements
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    T& set_name(const std::string& p_name) {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].set_name(get_name(p_name, i));
      }
      return ref;
    }

    /**
     * @brief      Clear elements name
     *
     * @return     reference to current object
     */
    T& clr_name() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].clr_name();
      }
      return ref;
    }

    /**
     * @}
     */

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
     * @brief      Set element names and read them
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    T& read(const std::string& p_name) {
      return set_name(p_name).read();
    }

    /**
     * @brief      Write elements using their names
     *
     * @return     reference to current object
     */
    T& write() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].write();
      }
      return ref;
    }

    /**
     * @brief      Set element names and write them
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    T& write(const std::string& p_name) {
      return set_name(p_name).write();
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
    void decrypt() {
      T& ref = *static_cast<T*>(this);
      for (unsigned i = 0; i < ref.size(); ++i) {
        ref[i].decrypt();
      }
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
    std::string get_name(const std::string& p_name, const unsigned p_order) {
      constexpr static int m_name_size_max = 256;
      static char buf[m_name_size_max];
      std::snprintf(buf, m_name_size_max, m_name_fmt.c_str(), p_name.c_str(), p_order);
      return buf;
    }
  };
} // cingulata

#endif // IO_NAME_VEC
