#ifndef BIT_EXEC_MAN
#define BIT_EXEC_MAN

#include <bit_exec/interface.hxx>

#include <string>

namespace cingulata
{
namespace internal
{
  /**
   * @brief      Bit-executor management class
   */
  class BitExecMan
  {
  public:
    /**
     * @brief      Set bit executor
     *
     * @param[in]  p_bit_exec  bit executor object
     */
    static void set_bit_exec(IBitExec *const p_bit_exec) {
      m_bit_exec = p_bit_exec;
    }

    /**
     * @brief      Get bit executor casted to given type
     *
     * @tparam     T     Return type
     *
     * @return     const pointer to casted bit executor object
     */
    template<typename T = IBitExec>
    static T *const get_bit_exec() {
      return static_cast<T*>(m_bit_exec);
    }

  protected:
    static IBitExec* m_bit_exec;
  };
} // namespace internal
} // namespace cingulata

#endif // BIT_EXEC_MAN
