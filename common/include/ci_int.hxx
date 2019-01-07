#ifndef CI_INT
#define CI_INT

#include <ci_bit.hxx>
#include <int_op_gen/interface.hxx>
#include <bit_exec/interface.hxx>

#include <vector>

namespace cingulata
{

  /**
   * @brief      Instrumented integer class.
   * @details    This class stores internally an array of @c CiBit objects. All
   *             the operations are performed at bit level.
   *  - The bit-size and signedness cannot be changed once the object has been
   *    constructed. Utility functions (#resize, #cast, #change_sign etc.) are
   *    provided to obtain new objects with different bit-sizes and signs.
   *  - Indexing functions (#operator[]) use python style indexing: 0 is the
   *    LSB, 1 the second LSB, etc. and -1 is the MSB, -2 the second MSB, etc.
   *  - Two's complement representations is used for signed integers.
   */
  class CiInt
  {
  public:
    /**
     * @brief      Set bit-wise integer operation generator
     *
     * @param      p_int_op_gen  The generator
     */
    static void set_int_op_gen(IIntOpGen const* p_int_op_gen);

    /**
     * @brief      Return the current integer operation generator
     *
     * @return     a const pointer to integer operation generator
     */
    static IIntOpGen const* int_op_gen();

  private:
    static IIntOpGen const* m_int_op_gen;

  public:
    /**
     * Default value for the @c is_signed parameter used during object creation.
     */
    constexpr static bool default_is_signed = false;

    /**
     * @brief      Construct a new integer with given value, bit count and
     *             signedness
     * @details    Refer to function #encode_plain_val documentation for
     *             more details on conversion of primitive type value to bits.

     * @param      p_val        cleartext value
     * @param      p_bit_cnt    bit count, if none given the bit-size of @c
     *                          p_val is used
     * @param      p_is_signed  signedness of integer, if none given the @c
     *                          p_val signedness is used
     *
     * @tparam     T            an integral type
     */
    template<typename T>
    CiInt(
      const T p_val,
      const unsigned p_bit_cnt = sizeof(T)*8,
      const bool p_is_signed = std::is_signed<T>::value)
    ;

    /**
     * @brief      Construct a new integer from a single bit
     *
     * @param      p_bit        a bit object
     * @param      p_is_signed  signedness of integer
     */
    CiInt(const CiBit& p_bit, const bool p_is_signed = default_is_signed);

    /**
     * @brief      Construct a new integer from a vector of bits
     * @note       First bit of vector is the least significant one
     *
     * @param      p_bits       vector of bits
     * @param      p_is_signed  signedness of integer
     */
    CiInt(
      const std::vector<CiBit>& p_bits,
      const bool p_is_signed = default_is_signed
    );

    /**
     * @brief      Copy constructor - use default
     *
     * @param[in]  other  object to copy
     */
    CiInt(const CiInt& other) = default;

    /**
     * @brief      Assign a @c CiInt object to current object
     *
     * @param[in]  other  object to assign from
     *
     * @return     reference to current object
     */
    CiInt& operator= (const CiInt& other);

    /**
     * @brief      Assign a primitive integer to current object
     * @details    Refer to function #encode_plain_val documentation for
     *             more details on conversion of primitive type value to bits.
     *
     * @param[in]  p_val  value to assign from
     *
     * @tparam     T      type of primitive integer
     *
     * @return     reference to current object
     */
    template<typename T>
    CiInt& operator= (const T p_val);

    /**
     * @brief      Destroys the object.
     */
    ~CiInt() = default;

    /**
     * @brief      Explicit conversion to @c CiBit object
     * @details    Returns an "1" bit when current object is different from zero
     */
    explicit operator CiBit() const;

    /**
     * @brief      Cast current object to a @c CiInt object with different
     *             bit-size and signedness
     * @note       Bit-size is changed in first place and afterwards the sign.
     *             This is important for signed integers where the MSB is
     *             extended.
     *
     * @param[in]  p_bit_cnt    new bit-size
     * @param[in]  p_is_signed  new sign
     *
     * @return     new object
     */
    CiInt cast(const unsigned p_bit_cnt, const bool p_is_signed) const;

    /**
     * @brief      Is integer signed?
     *
     * @return     integer signedness (@c true when signed)
     */
    bool is_signed() const;

    /**
     * @brief      Return a copy of current integer with changed sign
     *
     * @param      p_is_signed  new signedness
     *
     * @return     new integer object
     */
    CiInt change_sign(const bool p_is_signed) const;

    /**
     * @brief      Return a signed copy of current integer
     *
     * @return     new signed integer object
     */
    CiInt to_signed() const;

    /**
     * @brief      Return a unsigned copy of current integer
     *
     * @return     new unsigned integer object
     */
    CiInt to_unsigned() const;

    /**
     * @brief      Get integer bit-size
     *
     * @return     bit-size
     */
    unsigned size() const;

    /**
     * @brief      Resize object to @c p_bit_cnt bits
     * @details    If new bit-size is smaller then most significant bits are
     *             deleted. Otherwise, integer is extended by @c CiBit returned
     *             by #sign function.
     *
     * @param      p_bit_cnt  new bit-size
     *
     * @return     new object
     */
    CiInt resize(const unsigned p_bit_cnt) const;

    /**
     * @name Bit selection/change functions
     * @{
     */

    /**
     * @brief      Bit selection operator
     * @details    Returns a const reference to the bit given by index @c p_idx.
     *             If index is bigger than object bit-size this method returns
     *             bit given by #sign function.
     *
     * @param      p_idx  index of the bit to select
     *
     * @return     const reference to selected bit
     */
    const CiBit& operator[](const int p_idx) const;

    /**
     * @brief      @copybrief operator[]
     * @details    This method can be used to set bits, so compared to previous
     *             one it throws an error when index is larger than object
     *             bit-size
     *
     * @param      p_idx  index of the bit to select
     *
     * @return     reference to selected bit
     */
    CiBit& operator[](const int p_idx);

    /**
     * @brief      Get the least significant bit
     *
     * @return     a const reference to the least significant bit
     */
    const CiBit&  lsb() const;

    /**
     * @brief      Get the least significant bit
     *
     * @return     a reference to the least significant bit
     */
    CiBit&        lsb();

    /**
     * @brief      Get the most significant bit
     *
     * @return     a const reference to the most significant bit
     */
    const CiBit&  msb() const;

    /**
     * @brief      Get the most significant bit
     *
     * @return     a reference to the most significant bit
     */
    CiBit&        msb();

    /**
     * @brief      Get the sign bit of current integer object
     * @details    If integer is signed then the most significant bit is
     *             returned otherwise "0" bit.
     *
     * @return     a const reference to the sign bit
     */
    const CiBit&  sign() const;

    /**
     * @brief      Get internal representation of bit vector
     *
     * @return     Reference to vector of @c CiBit objects
     */
    const std::vector<CiBit>& bits() const;
    /**
     * @}
     */

    /**
     * @name Arithmetic operators
     * @{
     */
    CiInt&  operator  +=  (const CiInt& other);
    CiInt&  operator  -=  (const CiInt& other);
    CiInt&  operator  *=  (const CiInt& other);
    // CiInt&  operator  /=  (const CiInt& other);
    // CiInt&  operator  %=  (const CiInt& other);

    CiInt&  operator  ++  ();
    CiInt&  operator  --  ();
    CiInt   operator  ++  (int);
    CiInt   operator  --  (int);
    /**
     * @}
     */

    /**
     * @name Bitwise operators
     * @{
     */
    CiInt&  operator  &=  (const CiInt& other);
    CiInt&  operator  |=  (const CiInt& other);
    CiInt&  operator  ^=  (const CiInt& other);
    /**
     * @}
     */

    /**
     * @name Bit shift/rotate functions
     * @{
     */

    /**
     * @brief      Left shift
     * @details    Shift object bits to left by @c pos positions. If @c pos is
     *             negative then a right shift by the negative amount @c pos is
     *             used. Zero bits are inserted to the right (on LSB position).
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  <<= (const int pos);

    /**
     * @brief      Right shift
     * @details    Shift object bits to right by @c pos positions. If @c pos is
     *             negative then a left shift by the negative amount @c pos is
     *             used. MSB bit is replicated if integer is signed, otherwise
     *             zero bit is inserted to the left (on MSB position).
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  >>= (const int pos);

    /**
     * @brief      Left rotate
     * @details    Rotate object bits to left by @c pos positions. If @c pos is
     *             negative then function #ror with negative @c pos is called.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiInt&  rol           (const int pos);

    /**
     * @brief      Right rotate
     * @details    Rotate object bits to right by @c pos positions. If @c pos is
     *             negative then function #rol with negative @c pos is called.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiInt&  ror           (const int pos);
    /**
     * @}
     */

  private:
    std::vector<CiBit> m_bits;
    bool m_is_signed;

    /**
     * @brief      Encodes an integer into bits
     *
     * @param      p_val      input integer
     * @param      p_bit_cnt  number of bits
     *
     * @tparam     T          type of @c p_val
     */
    template
    <
        typename T,
        typename = typename std::enable_if<std::is_integral<T>::value, T>::type
    >
    void encode_plain_val(const T p_val, const unsigned p_bit_cnt);

    /**
     * @brief      Change current object to new bit-size and signedness
     * @note       Bit-size is changed in first place and afterwards the sign.
     *
     * @param[in]  p_bit_cnt    new bit-size
     * @param[in]  p_is_signed  new sign
     *
     * @return     reference to current object
     */
    CiInt& alter(const unsigned p_bit_cnt, const bool p_is_signed);

    /**
     * @brief      Transform index from relative to absolute (python-like indexing)
     * @details    This function returns size()-idx when idx<0 and idx otherwise
     *
     * @param[in]  idx   index to transform
     *
     * @return     transformed index
     */
    unsigned idx_rel_to_abs(const int idx) const;
  };

  /* Logical operators */
  CiBit   operator  !   (const CiInt& lhs);
  CiBit   operator  &&  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  ||  (const CiInt& lhs, const CiInt& rhs);

  /* Arithmetic operators */
  CiInt   operator  +   (const CiInt& lhs);
  CiInt   operator  -   (const CiInt& lhs);
  CiInt   operator  +   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  -   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  *   (const CiInt& lhs, const CiInt& rhs);
  // CiInt   operator  /   (const CiInt& lhs, const CiInt& rhs);
  // CiInt   operator  %   (const CiInt& lhs, const CiInt& rhs);

  /* Bitwise logic */
  CiInt   operator  ~   (CiInt lhs);
  CiInt   operator  &   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  |   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  ^   (const CiInt& lhs, const CiInt& rhs);

  /* Bitwise shift */
  CiInt   operator  <<  (CiInt lhs, const unsigned& pos);
  CiInt   operator  >>  (CiInt lhs, const unsigned& pos);
  CiInt   rol           (CiInt lhs, const unsigned& pos);
  CiInt   ror           (CiInt lhs, const unsigned& pos);

  /* Relational operators */
  CiBit   operator  ==  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  !=  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  <   (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  >   (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  <=  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  >=  (const CiInt& lhs, const CiInt& rhs);

  #include <ci_int_defs.cxx>
}

#endif
