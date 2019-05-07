#ifndef BIT_TRACKER
#define BIT_TRACKER

#include <bit_exec/interface.hxx>
#include <bit_exec/mem_man_basic.hxx>

#include <iostream>
#include <vector>
#include <string>

namespace cingulata
{
  namespace BitTrackerInternal {
    class Node;
    enum class NodeType : uint8_t;
    enum class GateType : uint8_t;
  }
  namespace BTI = BitTrackerInternal;

  /**
   * @brief Implemenation class for bit execution interface which tracks
   *  bit operations. This class constructs a boolean circuit corresponding
   *  to every called interface operation.
   */
  class BitTracker : public IBitExec {
  public:
    ~BitTracker() override;

    /**
     * @brief Delete the boolean circuit constructed so far
     */
    void        reset       ()                                              override;

    ObjHandle   encode      (const bit_plain_t pt_val)                      override;
    ObjHandle   encrypt     (const bit_plain_t pt_val)                      override;
    bit_plain_t decrypt     (const ObjHandle& in1)                          override;
    ObjHandle   read        (const std::string& name)                       override;
    void        write       (const ObjHandle& in1, const std::string& name) override;

    ObjHandle   op_not      (const ObjHandle& in1)                          override;
    ObjHandle   op_and      (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_xor      (const ObjHandle& in1, const ObjHandle& in2)    override;

    ObjHandle   op_nand     (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_andyn    (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_andny    (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_or       (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_nor      (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_oryn     (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_orny     (const ObjHandle& in1, const ObjHandle& in2)    override;
    ObjHandle   op_xnor     (const ObjHandle& in1, const ObjHandle& in2)    override;

    ObjHandle   op_mux      (const ObjHandle& cond,
                              const ObjHandle& in1, const ObjHandle& in2)   override;

    void        export_blif (std::ostream& stream,
                              const std::string& model_name = "CIRCUIT");

    void        export_blif (const std::string& file_name,
                              const std::string& model_name = "CIRCUIT");

  protected:
    MemManBasic<Allocator<BTI::Node>> mm;

    ObjHandle   add_gate    (BTI::GateType gate_type,
                              const std::initializer_list<ObjHandleT<BTI::Node>> inps_p);
    ObjHandle   add_input   (const std::string& name = "");
    void        make_output (const ObjHandleT<BTI::Node>& hdl, const std::string& name = "");

    std::vector<ObjHandleT<BTI::Node>>  inputs;
    std::vector<ObjHandleT<BTI::Node>>  gates;
    std::vector<ObjHandleT<BTI::Node>>  outputs;
  };
}

#endif
