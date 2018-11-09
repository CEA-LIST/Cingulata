#ifndef CI_BIT
#define CI_BIT

#include <bit_exec/interface.hxx>
#include <bit_exec/obj_handle.hxx>

#include <string>
#include <cassert>

namespace cingulata
{
  /**
   * @brief Instrumented bit class
   * @details CiBit can be in clear or in encrypted state when obj_hdl is not null.
   *  Plain-text -- cihper-text operations are automatically optimized.
   */
  class CiBit
  {
  public:
    /**
     * @brief Get current bit executor
     */
    static std::shared_ptr<IBitExec> bit_exec() {
      return m_bit_exec;
    }

    /**
     * @brief Set bit executor
     */
    static void set_bit_exec(const std::shared_ptr<IBitExec>& p_bit_exec) {
      m_bit_exec = p_bit_exec;
    }

  private:
    static std::shared_ptr<IBitExec> m_bit_exec;

  public:
    using bit_plain_t = IBitExec::bit_plain_t;

    /**
     * @brief Construct an object in with given clear bit value
     */
    CiBit(const bit_plain_t pt_val_p = 0) : pt_val(pt_val_p) {}

    /**
     * @brief Copy-construct an object
     */
    CiBit(const CiBit& other) {
      copy(other);
    }

    /**
     * @brief Move-construct an object
     */
    CiBit(CiBit&& other) {
      move(other);
    }

    /**
     * @brief Assignement operator overload
     */
    CiBit& operator=(const CiBit& other) {
      if (this != &other)
        copy(other);
      return *this;
    }

    /**
     * @brief Move-assignment operator overload
     */
    CiBit& operator=(CiBit&& other) {
      if (this != &other)
        move(other);
      return *this;
    }

    /**
     * @brief Destruct bit object
     */
    ~CiBit() = default;

    /**
     * @brief Get name, if name was not set before then a new name is generated automatically
     */
    std::string get_name(const std::string& prefix = "n") {
      if (name.empty())
        name = prefix + std::to_string(unique_name_cnt++);
      return name;
    }

    /**
     * @brief Set name
     */
    CiBit& set_name(const std::string& name_p) {
      name = name_p;
      return *this;
    }

    /**
     * @brief Clear name and set it to default value
     */
    CiBit& clr_name() {
      name.clear();
      return *this;
    }

    /**
     * @brief Returns clear bit value
     * @details If current object is in encrypted state the result is undefined
     * @return Plaintext value
     */
    bit_plain_t get_val() const {
      return pt_val;
    }

    /**
     * @brief Set bit to a plaintext value
     * @details If bit is in encrypted state, the underlying object handle is destroyed
     * @return Reference to current object
     */
    CiBit& set_val(const bit_plain_t pt_val_p) {
      clear_obj_handle();
      pt_val = pt_val_p & 1;
      return *this;
    }

    /**
     * @brief Read bit, use current bit name
     * @return Reference to current object
     */
    CiBit& read();

    /**
     * @brief Set name and read bit
     * @return Reference to current object
     */
    CiBit& read(const std::string name_p) {
      return set_name(name_p).read();
    }

    /**
     * @brief Write bit, use current bit name
     * @return Reference to current object
     */
    CiBit& write();

    /**
     * @brief Set name and write bit
     * @return Reference to current object
     */
    CiBit& write(const std::string name_p) {
      return set_name(name_p).write();
    }

    /**
     * @brief Encrypt bit
     * @return Reference to current object
     */
    CiBit& encrypt();

    /**
     * @brief Decrypt bit
     * @details If bit is an clear state this method does nothing
     * @return CiBit plaintext value
     */
    bit_plain_t decrypt();

    /**
     * @brief Is bit non-encrypted?
     */
    bool is_plain() const {
      return obj_hdl.is_empty();
    }

    /**
     * All possible boolean operations with non-encrypted bit
     */
    CiBit&  op_and      (const bit_plain_t);
    CiBit&  op_nand     (const bit_plain_t);
    CiBit&  op_andny    (const bit_plain_t);
    CiBit&  op_andyn    (const bit_plain_t);
    CiBit&  op_or       (const bit_plain_t);
    CiBit&  op_nor      (const bit_plain_t);
    CiBit&  op_orny     (const bit_plain_t);
    CiBit&  op_oryn     (const bit_plain_t);
    CiBit&  op_xor      (const bit_plain_t);
    CiBit&  op_xnor     (const bit_plain_t);

    /**
     * All possible boolean operations
     */
    CiBit&  op_not      ();
    CiBit&  op_and      (const CiBit&);
    CiBit&  op_nand     (const CiBit&);
    CiBit&  op_andny    (const CiBit&);
    CiBit&  op_andyn    (const CiBit&);
    CiBit&  op_or       (const CiBit&);
    CiBit&  op_nor      (const CiBit&);
    CiBit&  op_orny     (const CiBit&);
    CiBit&  op_oryn     (const CiBit&);
    CiBit&  op_xor      (const CiBit&);
    CiBit&  op_xnor     (const CiBit&);

    /**
     * Compound assignment operators overload
     */
    CiBit&  operator  +=  (const CiBit& rhs) { return op_or(rhs); }
    CiBit&  operator  -=  (const CiBit& rhs) { return op_xor(rhs); }
    CiBit&  operator  *=  (const CiBit& rhs) { return op_and(rhs); }
    CiBit&  operator  &=  (const CiBit& rhs) { return op_and(rhs); }
    CiBit&  operator  |=  (const CiBit& rhs) { return op_or(rhs); }
    CiBit&  operator  ^=  (const CiBit& rhs) { return op_xor(rhs); }

  protected:
    /**
     * @brief clear underlying object handle
     */
    void clear_obj_handle();

    /**
     * @brief Copy \c other object to current object
     */
    void copy(const CiBit& other);

    /**
     * @brief Move \c other object to current object
     */
    void move(const CiBit& other);

    static bit_plain_t negate(const bit_plain_t pt_val_p) {
      return 1^pt_val_p;
    }

  protected:
    static uint unique_name_cnt;

    bit_plain_t pt_val;
    ObjHandle obj_hdl;
    std::string name;
  };

  /**
   * Arithmetic operators
   */
  CiBit operator  +   (CiBit lhs, const CiBit& rhs);
  CiBit operator  -   (CiBit lhs, const CiBit& rhs);
  CiBit operator  *   (CiBit lhs, const CiBit& rhs);
  CiBit operator  ^   (CiBit lhs, const CiBit& rhs);
  CiBit operator  &   (CiBit lhs, const CiBit& rhs);
  CiBit operator  |   (CiBit lhs, const CiBit& rhs);

  /**
   * Logic operators
   */
  CiBit operator  !   (CiBit lhs);
  CiBit operator  &&  (CiBit lhs, const CiBit& rhs);
  CiBit operator  ||  (CiBit lhs, const CiBit& rhs);

  /**
   * Comparison operators
   */
  CiBit operator  ==  (CiBit lhs, const CiBit& rhs);
  CiBit operator  !=  (CiBit lhs, const CiBit& rhs);
  CiBit operator  <   (CiBit lhs, const CiBit& rhs);
  CiBit operator  <=  (CiBit lhs, const CiBit& rhs);
  CiBit operator  >   (CiBit lhs, const CiBit& rhs);
  CiBit operator  >=  (CiBit lhs, const CiBit& rhs);
}

#endif
