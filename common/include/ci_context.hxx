#ifndef CONTEXT
#define CONTEXT

#include <bit_exec/interface.hxx>
#include <int_op_gen/interface.hxx>

#include <string>
#include <cassert>

namespace cingulata
{
  /**
   * @brief      Context
   */
  class CiContext
  {
  public:
    /**
     * @brief      Set bit executor
     *
     * @param[in]  p_bit_exec  bit executor object
     */
    static void set_bit_exec(IBitExec* p_bit_exec) {
      m_bit_exec = p_bit_exec;
    }

    /**
     * @brief      Get bit executor casted to given type
     *
     * @tparam     T     Return type
     *
     * @return     pointer to casted bit executor object
     */
    template<typename T>
    static T* get_bit_exec_t() {
      return dynamic_cast<T*>(get_bit_exec());
    }

    /**
     * @brief      Get bit executor
     *
     * @return     pointer to bit executor object
     */
    static IBitExec* get_bit_exec() {
      assert(m_bit_exec != nullptr);
      return m_bit_exec;
    }

    /**
     * @brief      Set bit-wise integer operation generator
     *
     * @param      p_int_op_gen  The generator
     */
    static void set_int_op_gen(IIntOpGen* p_int_op_gen) {
      m_int_op_gen = p_int_op_gen;
    }

    /**
     * @brief      Get int operation generator casted to given type
     *
     * @tparam     T     Return type
     *
     * @return     pointer to casted integer operation generator object
     */
    template<typename T = IIntOpGen>
    static T* get_int_op_gen_t() {
      assert(m_int_op_gen != nullptr);
      return dynamic_cast<T*>(get_bit_exec());
    }

    /**
     * @brief      Get int operation generator
     *
     * @return     pointer to integer operation generator object
     */
    static IIntOpGen* get_int_op_gen() {
      assert(m_int_op_gen != nullptr);
      return m_int_op_gen;
    }

    static void set_config(IBitExec* p_bit_exec, IIntOpGen* p_int_op_gen) {
      set_bit_exec(p_bit_exec);
      set_int_op_gen(p_int_op_gen);
    }

  protected:
    static IBitExec* m_bit_exec;
    static IIntOpGen* m_int_op_gen;
  };
} // namespace cingulata

#endif // CONTEXT
