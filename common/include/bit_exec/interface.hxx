#ifndef BIT_EXEC_INTERFACE
#define BIT_EXEC_INTERFACE

#include <bit_exec/obj_handle.hxx>
#include <utils.hxx>

#include <memory>
#include <stdint.h>
#include <string>

namespace cingulata {
/**
 * @brief Abstract interface to bit execution modules
 * @details This interface is adapted for HE schemes with boolean plaintext
 *  space, bit tracking modules, etc.
 * The inputs and outputs of interface operations are generic object handles.
 * Each operation creates a new object handle. Handles point to object
 *  created by methods \c new_obj and \c del_obj. These methods can be
 *  overloaded in a child class, otherwise the default implementation creates
 *  empty handles.
 */
class IBitExec {
public:
  /* clang-format off */

  IBitExec()          = default;
  virtual ~IBitExec() = default;

  /**
   * @brief Initialize executor
   *  This method initializes internal state of executor.
   *  It should be overloaded in child-classes with state.
   */
  virtual void        init        () {}

  /**
   * @brief Reset the internal state of executor,
   *  if it has one
   */
  virtual void        reset       () {}

  /**
   * encode, encrypt, decrypt and IO operations
   */
  virtual ObjHandle   encode      (const bit_plain_t pt_val = 0)                  = 0;
  virtual ObjHandle   encrypt     (const bit_plain_t pt_val = 0)                  = 0;
  virtual bit_plain_t decrypt     (const ObjHandle& in1)                          = 0;
  virtual ObjHandle   read        (const std::string& name)                       = 0;
  virtual void        write       (const ObjHandle& in1, const std::string& name) = 0;

  virtual ObjHandle   op_not      (const ObjHandle& in1);

  /* Only AND and XOR gates are mandatory */
  virtual ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    = 0;
  virtual ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    = 0;

  virtual ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2);
  virtual ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2);

  /** Ternary operator cond?in1:in2, i.e. oblivious select */
  virtual ObjHandle   op_mux      (const ObjHandle& cond,
                                    const ObjHandle& in1, const ObjHandle& in2);

  /* clang-format on */
};
} // namespace cingulata

#endif
