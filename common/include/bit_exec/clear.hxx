#ifndef BIT_EXEC_PLAIN
#define BIT_EXEC_PLAIN

#include <bit_exec/interface.hxx>
#include <bit_exec/obj_man/allocator.hxx>
#include <bit_exec/obj_man/basic.hxx>

namespace cingulata {

/**
 * @brief      Implementation of @c IBitExec for clear-text execution
 * @details    This class can be used to execute the same operations on several
 *             bits (each bit in @c bit_plain_t type) in parallel, aka SIMD or
 *             bit-sliced execution.
 */
class BitExecClear : public IBitExec {
public:
  ObjHandle encode(const bit_plain_t pt_val) override;
  ObjHandle encrypt(const bit_plain_t pt_val) override;
  bit_plain_t decrypt(const ObjHandle &in1) override;
  ObjHandle read(const std::string &name) override;
  void write(const ObjHandle &in1, const std::string &name) override;

  ObjHandle op_and(const ObjHandle &in1, const ObjHandle &in2) override;
  ObjHandle op_xor(const ObjHandle &in1, const ObjHandle &in2) override;

protected:
  obj_man::Basic<obj_man::Allocator<bit_plain_t>> mm;
};

} // namespace cingulata

#endif
