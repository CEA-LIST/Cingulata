#ifndef CI_BIT
#define CI_BIT

#include <string>
#include <bit_exec/interface.hxx>

namespace cingulata
{
  /**
   * @brief      Instrumented bit class
   * @details    - CiBit object can be in clear or in encrypted state when @c
   *             obj_hdl member is not empty.
   *             - Operations between plain-text and cipher-text values are
   *               automatically optimized.
   *             - A string name can be assigned to a @c CiBit object using
   *               constructor or @c set_name function. This name follows object
   *               lifetime.
   */
  class CiBit
  {
  public:
    using bit_plain_t = IBitExec::bit_plain_t;

    static const CiBit zero;
    static const CiBit one;

    /**
     * @brief      Construct an object with a given clear value and a name.
     *
     * @param[in]  pt_val_p  plain-text value (optional)
     * @param[in]  name      name of new object (optional)
     */
    CiBit(const bit_plain_t pt_val_p = 0, const std::string& name = "");

    /**
     * @brief      Copy-construct an object
     * @note       The name of other object is not copied, thus the newly
     *             created object has an empty name.
     *
     * @param[in]  other  object to copy
     */
    CiBit(const CiBit& other);

    /**
     * @brief      Move and construct an object
     *
     * @param[in]  other  object to move
     */
    CiBit(CiBit&& other);

    /**
     * @brief      Assignment operator
     * @note       The name of other object is not copied, thus the newly
     *             created object has an empty name.
     *
     * @param[in]  other  object to copy
     *
     * @return     reference to current object
     */
    CiBit& operator=(const CiBit& other);

    /**
     * @brief      Move-assignment operator
     *
     * @param[in]  other  object to move
     *
     * @return     reference to current object
     */
    CiBit& operator=(CiBit&& other);

    /**
     * @brief      Destroys the object
     */
    ~CiBit() = default;

    /**
     * @brief      Get bit name if set beforehand or empty string otherwise.
     *
     * @return     object name
     */
    std::string get_name() const;

    /**
     * @brief      Set bit name
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    CiBit& set_name(const std::string& p_name);

    /**
     * @brief      Clear name and set it to default value
     *
     * @return     reference to current object
     */
    CiBit& clr_name();

    /**
     * @brief      Returns plain-text bit value
     * @details    If current object is in encrypted state the result is
     *             undefined
     *
     * @return     plain-text value
     */
    bit_plain_t get_val() const;

    /**
     * @brief      Set bit to a plaintext value
     * @details    If bit is in encrypted state, the underlying object handle is
     *             destroyed
     *
     * @param[in]  pt_val_p  plain-text value to use
     *
     * @return     reference to current object
     */
    CiBit& set_val(const bit_plain_t pt_val_p);

    /**
     * @brief      Read bit, use current bit name
     *
     * @return     reference to current object
     */
    CiBit& read();

    /**
     * @brief      Set name and read bit
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    CiBit& read(const std::string& p_name);

    /**
     * @brief      Write bit, use current bit name
     *
     * @return     reference to current object
     */
    const CiBit& write() const;

    /**
     * @brief      Set name and write bit
     *
     * @param[in]  p_name  string name to use
     *
     * @return     reference to current object
     */
    CiBit& write(const std::string& p_name);

    /**
     * @brief      Encrypt bit
     * @details    Bit object state pass to "encrypted"
     *
     * @return     reference to current object
     */
    CiBit& encrypt();

    /**
     * @brief      Decrypt bit
     * @details    Bit object state pass to "unencrypted". If bit is an clear
     *             state this method does nothing
     *
     * @return     plain-text value
     */
    bit_plain_t decrypt();

    /**
     * @brief      Is bit plain?
     *
     * @return     true if plain, false otherwise
     */
    bool is_plain() const;

    /**
     * @name Boolean operations with plain-text input
     * @{
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
     * @}
     */

    /**
     * @name Boolean operations
     * @{
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
     * @}
     */

    /**
     * @name Compound assignment operators
     * @{
     */
    CiBit&  operator  +=  (const CiBit&);
    CiBit&  operator  -=  (const CiBit&);
    CiBit&  operator  *=  (const CiBit&);
    CiBit&  operator  &=  (const CiBit&);
    CiBit&  operator  |=  (const CiBit&);
    CiBit&  operator  ^=  (const CiBit&);
    /**
     * @}
     */

  protected:
    /**
     * @brief      Clear underlying object handle
     */
    void clear_obj_handle();

    /**
     * @brief      Move object to current object
     *
     * @param[in]  other  object to move
     */
    void move(const CiBit& other);

    /**
     * @brief      Negate plain-text value
     *
     * @param[in]  pt_val_p  input value
     *
     * @return     negated input
     */
    static bit_plain_t negate(const bit_plain_t pt_val_p);

  protected:
    bit_plain_t pt_val;
    ObjHandle obj_hdl;
    std::string name;
  };

  /**
   * @name Arithmetic operators
   * @{
   */
  CiBit operator  +   (CiBit, const CiBit&);
  CiBit operator  -   (CiBit, const CiBit&);
  CiBit operator  *   (CiBit, const CiBit&);
  CiBit operator  ~   (CiBit);
  CiBit operator  ^   (CiBit, const CiBit&);
  CiBit operator  &   (CiBit, const CiBit&);
  CiBit operator  |   (CiBit, const CiBit&);
  /**
   * @}
   */

  /**
   * @name Boolean operators
   * @{
   */
  CiBit op_not      (CiBit);
  CiBit op_and      (CiBit, const CiBit&);
  CiBit op_nand     (CiBit, const CiBit&);
  CiBit op_andny    (CiBit, const CiBit&);
  CiBit op_andyn    (CiBit, const CiBit&);
  CiBit op_or       (CiBit, const CiBit&);
  CiBit op_nor      (CiBit, const CiBit&);
  CiBit op_orny     (CiBit, const CiBit&);
  CiBit op_oryn     (CiBit, const CiBit&);
  CiBit op_xor      (CiBit, const CiBit&);
  CiBit op_xnor     (CiBit, const CiBit&);
  /**
   * @}
   */

  /**
   * @name Logic operators
   * @{
   */
  CiBit operator  !   (CiBit);
  CiBit operator  &&  (CiBit, const CiBit&);
  CiBit operator  ||  (CiBit, const CiBit&);
  /**
   * @}
   */

  /**
   * @name Comparison operators
   * @{
   */
  CiBit operator  ==  (CiBit, const CiBit&);
  CiBit operator  !=  (CiBit, const CiBit&);
  CiBit operator  <   (CiBit, const CiBit&);
  CiBit operator  <=  (CiBit, const CiBit&);
  CiBit operator  >   (CiBit, const CiBit&);
  CiBit operator  >=  (CiBit, const CiBit&);
  /**
   * @}
   */

  /**
   * @name Stream input/output operators
   * @{
   */
  std::istream& operator>>(std::istream&, CiBit&);
  std::ostream& operator<<(std::ostream&, const CiBit&);
  /**
   * @}
   */
}

#endif
