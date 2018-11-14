#ifndef CI_INT
#define CI_INT

#include <ci_bit.hxx>
#include <int_op_gen/interface.hxx>
#include <bit_exec/interface.hxx>

#include <vector>
#include <memory>
#include <cstdint>

namespace cingulata
{

  /**
   * @brief      Instrumented integer class
   * @details    [long description]
   *  - only @c resize function changes the bit count of current object
   *  - python style indexing is used, thus -1 corresponds to MSB, -2 to second
   *    MSB, etc.
   */
  class CiInt
  {
  public:
    /**
     * @brief Set bit executor
     */
    static void set_int_op_gen(IIntOpGen const* p_int_op_gen) {
      m_int_op_gen = p_int_op_gen;
    }

  private:
    static IIntOpGen const* m_int_op_gen;

  public:
    constexpr static bool default_is_signed = false;

    /**
     * @brief      Construct a new integer with given value, bit count and
     *             signedness
     *
     * @param      p_val         cleartext value
     * @param      p_bit_cnt     bit count, if none given the @c p_val bit count
     *                           is used
     * @param      p_is_signed   signedness of integer, if none given the @c
     *                           p_val signedness is used
     *
     * @tparam     T             an integral type
     */
    template<typename T>
    CiInt(const T p_val, const uint32_t p_bit_cnt = sizeof(T)*8, const bool p_is_signed = std::is_signed<T>::value)
    :
      m_is_signed(p_is_signed)
    {
      encode_plain_val(p_val, p_bit_cnt);
    }

    /**
     * @brief      Construct a new integer from a single bit
     *
     * @param      p_bit        a bit object
     * @param      p_is_signed  signedness of integer
     */
    CiInt(const CiBit& p_bit, const bool p_is_signed = default_is_signed)
    :
      m_is_signed(p_is_signed)
    {
      m_bits.push_back(p_bit);
    }

    /**
     * @brief      Construct a new integer from a vector of bits
     * @note       First bit of vector is the least significant one
     *
     * @param      p_bits       vector of bits
     * @param      p_is_signed  signedness of integer
     */
    CiInt(const std::vector<CiBit>& p_bits, const bool p_is_signed = default_is_signed)
    :
      m_is_signed(p_is_signed)
    {
      m_bits = p_bits;
    }

    /**
     * @brief      Copy constructor
     *
     * @param[in]  other  object to copy
     */
    CiInt(const CiInt& other) = default;

    /**
     * @brief      Assignment operator
     *
     * @param[in]  other  object to copy
     *
     * @return     reference to current object
     */
    CiInt& operator= (const CiInt& other) {
      if (this != &other) {
        m_is_signed = other.m_is_signed;
        m_bits = other.m_bits;
      }
      return *this;
    }

    /**
     * @brief      Is current object signed?
     *
     * @return     true @c CiInt object signedness
     */
    bool is_signed() const { return m_is_signed; }

    /**
     * @brief      Transform @c CiInt to signed integer
     *
     * @return     reference to current object
     */
    CiInt& to_signed() { return change_sign(true); }

    /**
     * @brief      Transform @c CiInt to unsigned integer
     *
     * @return     reference to current object
     */
    CiInt& to_unsigned() { return change_sign(false); }

    /**
     * @brief      Get the number of bits in integer
     *
     * @return     bit count
     */
    uint32_t size() const { return m_bits.size(); }

    /**
     * @brief      Resize object to @c p_bit_cnt bits
     * @details    In case new bit count is smaller then most significant bits
     *             are deleted. Otherwise, integer is extended with zero bits in
     *             the unsigned case or with MSB bit in the signed case.
     *
     * @param      p_bit_cnt  new bit count
     *
     * @return     reference to current object
     */
    CiInt& resize(const uint32_t p_bit_cnt);

    /** @name Bit selection/change functions */
    /** @{ */

    /**
     * @brief      Bit selection operator
     * @details    Returns a copy of a specific bit If bit index @c p_idx is
     *             bigger than object bit-size this method returns:
     *              - a copy of last bit (sign bit) for signed objects
     *              - bit otherwise
     *
     * @param      p_idx  index of the bit to return
     *
     * @return     bit copy
     */
    const CiBit operator[](const int32_t p_idx) const;

    /**
     * @brief      Bit selection operator
     * @details    This method can be used to set bits, so compared to previous
     *             one it throws an error when index is
     *             larger than object bit-size
     *
     * @param      p_idx  index of the bit to return
     *
     * @return     bit reference
     */
    CiBit& operator[](const int32_t p_idx);

    CiBit&      lsb()       { return (*this)[0]; }
    const CiBit lsb() const { return (*this)[0]; }
    CiBit&      msb()       { return (*this)[-1]; }
    const CiBit msb() const { return (*this)[-1]; }
    /** @} */

    /** @name Arithmetic operators */
    /** @{ */
    CiInt&  operator  +=  (const CiInt& other);
    CiInt&  operator  -=  (const CiInt& other);
    CiInt&  operator  *=  (const CiInt& other);
    // CiInt&  operator  /=  (const CiInt& other);
    // CiInt&  operator  %=  (const CiInt& other);

    CiInt&  operator  ++  ();
    CiInt&  operator  --  ();
    CiInt   operator  ++  (int);
    CiInt   operator  --  (int);
    /** @} */

    /** @name Bitwise operators */
    /** @{ */
    CiInt&  operator  &=  (const CiInt& other);
    CiInt&  operator  |=  (const CiInt& other);
    CiInt&  operator  ^=  (const CiInt& other);
    /** @} */

    /** @name Bit shift/rotate functions */
    /** @{ */
    /**
     * @brief      Left shift
     * @details    Shift object bits to left by @c pos positions. If @c pos is
     *             negative then a right shift by the negative amount @c pos is
     *             used. Zero bits are inserted on the rights (LSB position).
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  <<= (const int32_t pos);

    /**
     * @brief      Right shift
     * @details    Shift object bits to right by @c pos positions. If @c pos is
     *             negative then a left shift by the negative amount @c pos is
     *             used. MSB bit is replicated if @c CiInt is signed, otherwise
     *             zero bit is inserted on the left (MSB position).
     *
     * @param[in]  pos   number of positions to shift
     *
     * @return     reference to current object
     */
    CiInt&  operator  >>= (const int32_t pos);


    /**
     * @brief      Left rotate
     * @details    Rotate object bits to left by @c pos positions. If @c pos is
     *             negative then a right rotate by the negative amount @c pos is
     *             used.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiInt&  rol           (const int32_t pos);

    /**
     * @brief      Right rotate
     * @details    Rotate object bits to right by @c pos positions. If @c pos is
     *             negative then a left rotate by the negative amount @c pos is
     *             used.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiInt&  ror           (const int32_t pos);
    /** @} */

  private:
    std::vector<CiBit> m_bits;
    bool m_is_signed;

    /**
     * @brief      Change the sign of current object
     *
     * @param      p_is_signed  new signedness
     *
     * @return     reference to current object
     */
    CiInt& change_sign(const bool p_is_signed) {
      m_is_signed = p_is_signed;
      return *this;
    }

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
    void encode_plain_val(const T p_val, const uint32_t p_bit_cnt);

    uint32_t idx_rel_to_abs(const int32_t idx) const {
      return (idx < 0) ? size()+idx : idx;
    }
  };

  /* logical */
  CiBit   operator  !   (CiInt lhs);
  CiBit   operator  &&  (CiInt lhs, const CiInt& rhs);
  CiBit   operator  ||  (CiInt lhs, const CiInt& rhs);

  /* arithmetic */
  CiInt   operator  +   (CiInt lhs)                         { return lhs; }
  CiInt   operator  -   (CiInt lhs);
  CiInt   operator  +   (CiInt lhs, const CiInt& rhs)       { return lhs += rhs; }
  CiInt   operator  -   (CiInt lhs, const CiInt& rhs)       { return lhs -= rhs; }
  CiInt   operator  *   (CiInt lhs, const CiInt& rhs)       { return lhs *= rhs; }
  // CiInt   operator  /   (CiInt lhs, const CiInt& rhs)    { return lhs /= rhs; }
  // CiInt   operator  %   (CiInt lhs, const CiInt& rhs)    { return lhs %= rhs; }

  /* Bitwise logic */
  CiInt   operator  ~   (CiInt lhs);
  CiInt   operator  &   (CiInt lhs, const CiInt& rhs)       { return lhs &= rhs; }
  CiInt   operator  |   (CiInt lhs, const CiInt& rhs)       { return lhs |= rhs; }
  CiInt   operator  ^   (CiInt lhs, const CiInt& rhs)       { return lhs ^= rhs; }

  /* Bitwise shift */
  CiInt   operator  <<  (CiInt lhs, const uint32_t& pos) { return lhs <<= pos; }
  CiInt   operator  >>  (CiInt lhs, const uint32_t& pos) { return lhs >>= pos; }
  CiInt   rol           (CiInt lhs, const uint32_t& pos) { return lhs.rol(pos); }
  CiInt   ror           (CiInt lhs, const uint32_t& pos) { return lhs.ror(pos); }

  /* Relational operators */
  CiBit   operator  ==  (CiInt lhs, const CiInt& rhs);
  CiBit   operator  !=  (CiInt lhs, const CiInt& rhs);
  CiBit   operator  <   (CiInt lhs, const CiInt& rhs);
  CiBit   operator  >   (CiInt lhs, const CiInt& rhs);
  CiBit   operator  <=  (CiInt lhs, const CiInt& rhs);
  CiBit   operator  >=  (CiInt lhs, const CiInt& rhs);
}

// #include "ci_int_impl.cxx"

#endif
