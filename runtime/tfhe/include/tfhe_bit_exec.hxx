#ifndef TFHE_EXECUTOR
#define TFHE_EXECUTOR

#include <bit_exec/interface.hxx>
#include <tfhe.h>

namespace cingulata {

  /**
   * @brief      Bit executor implemenation for TFHE library.
   */
  class TfheBitExec : public IBitExec {
  public:
    TfheBitExec(
        const TFheGateBootstrappingCloudKeySet *const p_pks,
        const TFheGateBootstrappingSecretKeySet *const p_sks = nullptr);

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
  protected:
    void*       new_obj     ()                                              override;
    void        del_obj     (void * obj_ptr)                                override;

    const TFheGateBootstrappingCloudKeySet *const public_keyset;
    const TFheGateBootstrappingSecretKeySet *const secret_keyset;
  };
};

#endif
