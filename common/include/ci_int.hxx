/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#ifndef CI_INT
#define CI_INT

#include <ci_bit.hxx>
#include <ci_bit_vector.hxx>
#include <io_name_vec.hxx>
#include <utils.hxx>

#include <vector>
#include <cmath>

namespace cingulata
{

  /**
   * @brief      An integer class with @c CiBit elements.
   * @details    All the operations are performed at bit level. This class uses
   *             internally a vector of type @c CiBitVector.
   *  - The bit-size and signedness cannot be changed once the object has been
   *    created. Utility functions (#resize, #alter, #change_sign, #to_signed
   *    and #to_unsigned.) are provided to obtain new objects with different
   *    bit-sizes and signedness.
   *  - Indexing functions (#operator[]) use python style indexing: 0 is the
   *    LSB, 1 the second LSB, etc. and -1 is the MSB, -2 the second MSB, etc.
   *  - Two's complement representations is used for signed integers.
   */
  class CiInt : public IoNameVec<CiInt>
  {
  public:
    /**
     * Default value for the @c is_signed parameter used during object creation.
     */
    constexpr static bool default_is_signed = false;

    /**
     * @name Helper functions for @c CiInt objects creation
     * @{
     */
    /**
     * Functions to create @c CiInt object from a native integer and predefined sizes and signedness.
     */
    static CiInt u8v(const uint8_t = 0);
    static CiInt u16v(const uint16_t = 0);
    static CiInt u32v(const uint32_t = 0);
    static CiInt u64v(const uint64_t = 0);

    static CiInt s8v(const int8_t = 0);
    static CiInt s16v(const int16_t = 0);
    static CiInt s32v(const int32_t = 0);
    static CiInt s64v(const int64_t = 0);

    /**
     * Zero encoding objects of different sizes and signedness. Provided to
     * facilitate the creation of native type equivalents of @c CiInt objects.
     */
    static const CiInt u8;
    static const CiInt u16;
    static const CiInt u32;
    static const CiInt u64;

    static const CiInt s8;
    static const CiInt s16;
    static const CiInt s32;
    static const CiInt s64;
    /**
     * @}
     */

    /**
     * @brief      Construct a new object from a native integer with a given bit
     *             count and signedness
     * @details    Value @c p_val is converted to a bit vector using method
     *             @link utils.hxx encode_plain_int()
     *             @endlink. Refer to its documentation for more details on
     *             conversion rules of primitive type values to bits.
     *
     * @param      p_val        integer value to encode
     * @param      p_bit_cnt    bit count, if -1 (default value) given then the
     *                          minimal bit-size needed to encode @c p_val is
     *                          used
     * @param      p_is_signed  signedness of new object, if none given the @c
     *                          default_is_signed value is used
     */
    CiInt(
      const int64_t p_val,
      const unsigned p_bit_cnt = (unsigned)-1,
      const bool p_is_signed = default_is_signed
    );

    /**
     * @brief      Construct a new integer containing @c p_bit_cnt copies of @c
     *             p_bit
     *
     * @param      p_bit        a bit object
     * @param[in]  p_bit_cnt    number of bits to copy
     * @param      p_is_signed  signedness of new object, if none given the @c
     *                          default_is_signed value is used
     */
    CiInt(
      const CiBit& p_bit,
      const unsigned p_bit_cnt = 1,
      const bool p_is_signed = default_is_signed
    );

    /**
     * @brief      Construct an object from a vector of bits
     * @note       First bit of vector is the least significant one
     *
     * @param      p_bits       vector of bits
     * @param      p_is_signed  signedness of new object, if none given the @c
     *                          default_is_signed value is used
     */
    CiInt(
      const std::vector<CiBit>& p_bits,
      const bool p_is_signed = default_is_signed
    );

    /**
     * @brief      Constructs an object from a @c CiBitVector object
     * @note       First element of bit-vector @c p_bits is the least
     *             significant one
     *
     * @param[in]  p_bits       vector of bits object
     * @param[in]  p_is_signed  signedness of new object
     */
    CiInt(
      const CiBitVector& p_bits,
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
     * @details    Copy bits from @c other object to current one. Size and
     *             signedness of current object **do not change**. If @c other
     *             object size is larger than the size of the current object
     *             then only the first #size bits are copied, otherwise @c other
     *             object is extended to #size bits before copy.
     *
     * @param[in]  other  object to assign from
     *
     * @return     reference to current object
     */
    CiInt& operator= (const CiInt& other);

    /**
     * @brief      Assign a primitive type value to current object
     * @details    Current object bit-count and signedness are not changed.
     *             Function
     *             @link utils.hxx encode_plain_int()
     *             @endlink is used to encode native type value into bits.
     *
     * @param[in]  p_val      value to assign from
     *
     * @return     reference to current object
     */
    CiInt &operator=(const int64_t p_val);

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
     * @brief      Explicit conversion to @c CiBitVector object
     * @details    Returns the bit-vector of current object with LSB first.
     */
    explicit operator CiBitVector() const;

    /**
     * @brief      Get bit-vector representation of current object
     * @details    Resuling bit-vector with LSB first.
     *
     * @return     a bit-vector object
     */
    // const CiBitVector& bits() const;

    /**
     * @brief      Cast current object to a bit-vector object
     *
     * @return     new object
     */
    CiBitVector cast() const;

    /**
     * @brief      @copybrief cast()
     * @details    Bit-vector object size is changed to @c p_bit_cnt. When size
     *             is increased #sign bits are added to the end.
     *
     * @param[in]  p_bit_cnt  new bit-size
     *
     * @return     new object
     */
    CiBitVector cast(const unsigned p_bit_cnt) const;

    /**
     * @brief      Is integer signed?
     *
     * @return     integer signedness (@c true when signed)
     */
    bool is_signed() const;

    /**
     * @brief      Create a copy of current integer with sign changed
     *
     * @param      p_is_signed  new signedness
     *
     * @return     new integer object
     */
    CiInt change_sign(const bool p_is_signed) const;

    /**
     * @brief      Create a signed copy of current object
     *
     * @return     new signed integer object
     */
    CiInt to_signed() const;

    /**
     * @brief      Create a unsigned copy of current object
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
     * @brief      Create a resized copy of current object
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
     * @brief      Create new object from current one with new bit-size and
     *             signedness
     * @note       Bit-size is changed in first place and afterwards the sign.
     *
     * @param[in]  p_bit_cnt    new bit-size
     * @param[in]  p_is_signed  new sign
     *
     * @return     new object
     */
    CiInt alter(const unsigned p_bit_cnt, const bool p_is_signed) const;

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
     * @}
     */

    /**
     * @name Arithmetic operators
     * @{
     */
    CiInt   operator  +   () const;
    CiInt   operator  -   () const;
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
     * @details    Shift object bits to left by @c pos positions. Zero bits are
     *             inserted on LSB positions.
     * @note       If @c pos is negative then a right shift (#operator>>=) by the
     *             negative amount @c pos is used.
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  <<= (const int pos);

    /**
     * @brief      Right shift
     * @details    Shift object bits to right by @c pos positions. Bit given by
     *             #sign function is replicated on MSB positions.
     * @note       If @c pos is negative then a left shift (#operator<<=) by the
     *             negative amount @c pos is used.
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  >>= (const int pos);

    /**
     * @brief      Left rotate
     * @details    Rotate object bits to left by @c pos positions. This function
     *             moves the MSB bit to LSB position @c pos times.
     * @note       If @c pos is negative then a right rotate (#ror) with
     *             negative amount @c pos is used.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiInt&  rol           (const int pos);

    /**
     * @brief      Right rotate
     * @details    Rotate object bits to right by @c pos positions. This
     *             function moves the LSB bit to MSB position @c pos times.
     * @note       If @c pos is negative then a left rotate (#rol) with negative
     *             amount @c pos is used.
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
    CiBitVector m_bits;
    bool m_is_signed;
  };

  /* Logical operators */
  CiBit   operator  !   (const CiInt& lhs);
  CiBit   operator  &&  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  ||  (const CiInt& lhs, const CiInt& rhs);

  /* Arithmetic operators */
  CiInt   operator  +   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  -   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  *   (const CiInt& lhs, const CiInt& rhs);
  // CiInt   operator  /   (const CiInt& lhs, const CiInt& rhs);
  // CiInt   operator  %   (const CiInt& lhs, const CiInt& rhs);

  /* Bitwise logic */
  CiInt   operator  ~   (const CiInt& lhs);
  CiInt   operator  &   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  |   (const CiInt& lhs, const CiInt& rhs);
  CiInt   operator  ^   (const CiInt& lhs, const CiInt& rhs);

  /* Bitwise shift */
  CiInt   operator  <<  (CiInt lhs, const int pos);
  CiInt   operator  >>  (CiInt lhs, const int pos);
  CiInt   rol           (CiInt lhs, const int pos);
  CiInt   ror           (CiInt lhs, const int pos);

  /* Relational operators */
  CiBit   operator  ==  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  !=  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  <   (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  >   (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  <=  (const CiInt& lhs, const CiInt& rhs);
  CiBit   operator  >=  (const CiInt& lhs, const CiInt& rhs);

  /**
   * @name Stream input/output operators
   * @{
   */
  std::istream& operator>>(std::istream&, CiInt&);
  std::ostream& operator<<(std::ostream&, const CiInt&);
  /**
   * @}
   */

  /**
   * @brief      Gets the bit-size the output of a 2 input operator.
   * @details    This function computes the bit-size of the integer obtained as a
   *             result of application of an 2-input integer operator.
   *
   * @param[in]  lhs   The left hand side
   * @param[in]  rhs   The right hand side
   *
   * @return     Result bit-size
   */
  unsigned result_size(const CiInt& lhs, const CiInt& rhs);

  /**
   * @brief      Gets the sign the output of a 2 input operator.
   * @details    This function computes the signedness of the integer obtained as
   *             a result of application of an 2-input integer operator.
   *
   * @param[in]  lhs   The left hand side
   * @param[in]  rhs   The right hand side
   *
   * @return     Result sign
   */
  bool result_is_signed(const CiInt& lhs, const CiInt& rhs);
}

#endif
