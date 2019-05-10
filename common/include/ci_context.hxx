#ifndef CONTEXT
#define CONTEXT

#include <bit_exec/interface.hxx>
#include <int_op_gen/interface.hxx>

#include <cassert>
#include <string>

namespace cingulata {
/**
 * @brief      Context
 */
class CiContext {
public:
  /**
   * @brief      Set bit executor
   *
   * @param[in]  p_bit_exec  bit executor object
   */
  static void set_bit_exec(const std::shared_ptr<IBitExec> &p_bit_exec) {
    m_bit_exec = p_bit_exec;
  }

  /**
   * @brief      Clear current integer operation generator
   */
  static void clear_bit_exec() { m_bit_exec = std::shared_ptr<IBitExec>(); }

  /**
   * @brief      Get bit executor casted to given type or empty if dynamic
   *             cast fails.
   *
   * @tparam     T     Return type
   *
   * @return     shared pointer to casted bit executor object
   */
  template <typename T> static std::shared_ptr<T> get_bit_exec_t() {
    return std::dynamic_pointer_cast<T>(get_bit_exec());
  }

  /**
   * @brief      Get bit executor
   *
   * @return     shared pointer to bit executor object
   */
  static std::shared_ptr<IBitExec> get_bit_exec() { return m_bit_exec; }

  /**
   * @brief      Set bit-wise integer operation generator
   *
   * @param      p_int_op_gen  The generator
   */
  static void set_int_op_gen(const std::shared_ptr<IIntOpGen> &p_int_op_gen) {
    m_int_op_gen = p_int_op_gen;
  }

  /**
   * @brief      Clear current integer operation generator
   */
  static void clear_int_op_gen() {
    m_int_op_gen = std::shared_ptr<IIntOpGen>();
  }

  /**
   * @brief      Get int operation generator casted to given type
   *
   * @tparam     T     Return type
   *
   * @return     pointer to casted integer operation generator object
   */
  template <typename T> static std::shared_ptr<T> get_int_op_gen_t() {
    return std::dynamic_pointer_cast<T>(get_bit_exec());
  }

  /**
   * @brief      Get int operation generator
   *
   * @return     pointer to integer operation generator object
   */
  static std::shared_ptr<IIntOpGen> get_int_op_gen() { return m_int_op_gen; }

  static void set_config(const std::shared_ptr<IBitExec> &p_bit_exec,
                         const std::shared_ptr<IIntOpGen> &p_int_op_gen) {
    set_bit_exec(p_bit_exec);
    set_int_op_gen(p_int_op_gen);
  }

protected:
  static std::shared_ptr<IBitExec> m_bit_exec;
  static std::shared_ptr<IIntOpGen> m_int_op_gen;
};
} // namespace cingulata

#endif // CONTEXT
