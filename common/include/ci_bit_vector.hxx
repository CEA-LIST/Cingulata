#ifndef CI_BIT_VECTOR
#define CI_BIT_VECTOR

#include <ci_bit.hxx>
#include <io_name_vec.hxx>

#include <vector>

namespace cingulata
{
  /**
   * @brief      A vector of @c CiBit elements
   * @details    The size of current object is mutable (#resize function).
   *  - Indexing functions (#operator[], #slice) use python style indexing. Thus
   *    '-1' is the last bit, '-2' the second last bit, etc.
   */
  class CiBitVector : public IoNameVec<CiBitVector>
  {
  public:
    /**
     * @brief      Construct an object containing @c p_bit_cnt copies of @c
     *             p_bit.
     *
     * @param[in]  p_bit      @c CiBit object to copy
     * @param[in]  p_bit_cnt  size of bit-vector object to construct
     */
    CiBitVector(const int p_bit_cnt = 0, const CiBit& p_bit = CiBit::zero);

    /**
     * @brief      Construct a bit-vector from a vector of bits
     *
     * @param[in]  p_bits  initializer list of bits
     */
    CiBitVector(const std::vector<CiBit>& p_bits);

    /**
     * @brief      Copy constructor -- use default
     *
     * @param[in]  other  object to copy
     */
    CiBitVector(const CiBitVector& other) = default;

    /**
     * @brief      Assign a bit-vector object to current object
     *
     * @param[in]  other  object to assign from
     *
     * @return     reference to current object
     */
    CiBitVector& operator= (const CiBitVector& other);

    /**
     * @brief      Get the bit-vector size
     *
     * @return     number of bits in current object
     */
    unsigned size() const;

    /**
     * @brief      Resizes current object to @c p_bit_cnt bits
     * @details    If new bit-size is smaller then end bits are deleted.
     *             Otherwise, bit vector is extended with copies of @c p_bit.
     *
     * @param      p_bit_cnt  new bit size
     * @param[in]  p_bit      object to add when size is extended
     *
     * @return     reference to current object
     */
    CiBitVector& resize(const unsigned p_bit_cnt, const CiBit& p_bit = CiBit::zero);

    /**
     * @brief      Append a bit to the end of vector
     *
     * @param[in]  p_bit  The @c CiBit object to append
     *
     * @return     reference to current object
     */
    CiBitVector& append(const CiBit& p_bit);

    /**
     * @name Bit selection/change functions
     * @{
     */

    /**
     * @brief      Bit selection operator
     * @details    Returns a reference to the bit on position @c p_idx. Method
     *             @c idx_rel_to_abs is used to transform @c p_idx beforehand.
     *             If index is bigger than object size this method throws an
     *             error.
     *
     * @param      p_idx  position of bit to get
     *
     * @return     reference to selected bit
     */
    CiBit& operator[] (const int p_idx);

    /**
     * @copybrief operator[]
     * @details    Returns a reference to the bit on position @c p_idx. Method
     *             @c idx_rel_to_abs is used to transform @c p_idx beforehand.
     *             If index is bigger than object size a reference to @c
     *             CiBit::zero is returned.
     *
     * @param      p_idx  position of bit to get
     *
     * @return     const reference to selected bit
     */
    const CiBit& operator[] (const int p_idx) const;

    /**
     * @brief      Get bit at index @c p_idx
     * @details    Returns a reference to the bit on position @c p_idx. Method
     *             @c idx_rel_to_abs is used to transform @c p_idx beforehand.
     *             If index is bigger than object size this method throws an
     *             error.
     *
     * @param[in]  p_idx  bit position
     *
     * @return     reference to selected bit
     */
    CiBit& at(const int p_idx);

    /**
     * @brief      Get bit at index @c p_idx or @c p_bit if out of range
     * @details    Returns a reference to the bit on position @c p_idx. Method
     *             @c idx_rel_to_abs is used to transform @c p_idx beforehand.
     *             If index is bigger than object size a reference to @c
     *             p_bit is returned.
     *
     * @param[in]  p_idx  bit position
     * @param[in]  p_bit  default bit value
     *
     * @return     const reference to selected bit
     */
    const CiBit& at(const int p_idx, const CiBit& p_bit = CiBit::zero) const;

    /**
     * @brief      Select a range of bits using slice indexing.
     * @details    Bits on positions @c start+k*stride<end for @c pos>=0 are used
     *             to create a new bit-vector object. Method @c idx_rel_to_abs
     *             is used to transform @c start and @c end indexes beforehand.
     *
     * @param[in]  p_start   start index
     * @param[in]  p_end     end index (exclusive)
     * @param[in]  p_stride  stride
     *
     * @return     A new object with selected bits
     */
    CiBitVector slice(int p_start = 0, int p_end = 1<<30, const int p_stride = 1) const;

    /**
     * @}
     */

    /**
     * @name Bitwise operators
     * @{
     *
     * @brief      Perform operation on first @c min(size(),other.size()) bits
     *             individually.
     * @details    Other bits are remain unchanged.
     *
     * @param[in]  other  object
     *
     * @return     reference to current object
     */
    CiBitVector&  operator  &=  (const CiBitVector& other);
    CiBitVector&  operator  |=  (const CiBitVector& other);
    CiBitVector&  operator  ^=  (const CiBitVector& other);
     /**
       * @}
       */

    /**
     * @name Bitwise operators
     * @{
     *
     * @brief      Perform operation between current object elements and object
     *             @c p_bit
     *
     * @param[in]  p_bit  The p bit
     *
     * @return     reference to current object
     */
    CiBitVector&  operator  &=  (const CiBit& p_bit);
    CiBitVector&  operator  |=  (const CiBit& p_bit);
    CiBitVector&  operator  ^=  (const CiBit& p_bit);
     /**
       * @}
       */


    /**
     * @brief      Negate all bits of current object
     *
     * @return     reference to current object
     */
    CiBitVector&  op_not      ();

    /**
     * @name Bitwise boolean operations with a bit-vector
     * @{
     *
     * @brief      Perform operation bitwise.
     * @details    Extend @c other with copies of @c p_bit if the size of
     *             current object is larger then the size of @c other.
     *
     * @param[in]  other  bit-vector object
     * @param[in]  p_bit  bit to copy in case of @c other has smaller size than
     *                    current object
     *
     * @return     reference to current object
     */
    CiBitVector&  op_and      (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_nand     (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_andny    (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_andyn    (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_or       (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_nor      (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_orny     (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_oryn     (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_xor      (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    CiBitVector&  op_xnor     (const CiBitVector& other, const CiBit& p_bit = CiBit::zero);
    /**
     * @}
     */

    /**
     * @name Bitwise boolean operations with a bit
     * @{
     *
     * @brief      Perform operation bitwise between bit-vector elements and a
     *             bit.
     *
     * @param[in]  p_bit  bit object
     *
     * @return     reference to current object
     */
    CiBitVector&  op_and      (const CiBit& p_bit);
    CiBitVector&  op_nand     (const CiBit& p_bit);
    CiBitVector&  op_andny    (const CiBit& p_bit);
    CiBitVector&  op_andyn    (const CiBit& p_bit);
    CiBitVector&  op_or       (const CiBit& p_bit);
    CiBitVector&  op_nor      (const CiBit& p_bit);
    CiBitVector&  op_orny     (const CiBit& p_bit);
    CiBitVector&  op_oryn     (const CiBit& p_bit);
    CiBitVector&  op_xor      (const CiBit& p_bit);
    CiBitVector&  op_xnor     (const CiBit& p_bit);
    /**
     * @}
     */


    /**
     * @}
     */

    /**
     * @name Bit shift/rotate functions
     * @{
     */

    /**
     * @brief      Left shift object bits
     * @details    Shift bits to the left by @c pos positions. First @c pos bits
     *             (positions @c 0..pos-1) are deleted. @c pos copies of @c
     *             p_bit object are inserted on the last @c pos positions.
     * @note       If @c pos is negative then a right shift (#shr) by negative
     *             amount @c pos is used.
     *
     * @param[in]  pos    number of positions to shift
     * @param[in]  p_bit  bit object to copy
     *
     * @return     reference to current object
     */
    CiBitVector&  shl           (const int pos, const CiBit& p_bit = CiBit::zero);

    /**
     * @brief      Right shift object bits
     * @details    Shift object bits to the right by @c pos positions. Last @c
     *             pos bits (positions @c #size-pos..#size-1) are deleted.
     *             Copies of @c p_bit object are inserted on the first @c pos
     *             positions.
     * @note       If @c pos is negative then a left shift (#shl) by negative
     *             amount @c pos is used.
     *
     * @param[in]  pos    number of positions to shift
     * @param[in]  p_bit  bit object to copy
     *
     * @return     reference to current object
     */
    CiBitVector&  shr           (const int pos, const CiBit& p_bit = CiBit::zero);

    /**
     * @brief      Left rotate
     * @details    Rotate object bits to the left by @c pos positions. Bits on
     *             first @c pos positions are moved to the end of vector.
     * @note       If @c pos is negative then a right rotate (#ror) by negative
     *             amount @c pos is used.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiBitVector&  rol           (const int pos);

    /**
     * @brief      Right rotate
     * @details    Rotate object bits to the right by @c pos positions. Bits on
     *             last @c pos positions are moved to the beginning of vector.
     * @note       If @c pos is negative then a left rotate (#rol) by negative
     *             amount @c pos is used.
     *
     * @param[in]  pos   number of positions to rotate
     *
     * @return     reference to current object
     */
    CiBitVector&  ror           (const int pos);


    /**
     * @copydoc shl()
     */
    CiBitVector&  operator  <<= (const int pos);

    /**
     * @copydoc shr()
     */
    CiBitVector&  operator  >>= (const int pos);

    /**
     * @}
     */

  protected:
    /**
     * @brief      Transform @c idx from relative to absolute (python-like
     *             indexing)
     * @details    This function returns size()-idx when idx<0 and idx otherwise
     *
     * @param[in]  idx   value to transform
     *
     * @return     transformed value
     */
    int idx_rel_to_abs(const int idx) const;

    /**
     * @brief      Clips @c idx to range 0..#size-1
     *
     * @param[in]  idx   value to clip
     *
     * @return     clipped value
     */
    unsigned idx_clip(const int idx) const;

  private:
    std::vector<CiBit> m_vec;
  };

  /**
   * @name Bitwise boolean operations with a bit-vector
   * @{
   */
  CiBitVector   operator  ~   (CiBitVector lhs);
  CiBitVector   operator  ^   (CiBitVector, const CiBitVector&);
  CiBitVector   operator  &   (CiBitVector, const CiBitVector&);
  CiBitVector   operator  |   (CiBitVector, const CiBitVector&);
  /**
   * @}
   */

  /* Bitwise shift */
  CiBitVector   shl           (CiBitVector lhs, const int pos, const CiBit& p_bit);
  CiBitVector   shr           (CiBitVector lhs, const int pos, const CiBit& p_bit);
  CiBitVector   rol           (CiBitVector lhs, const int pos);
  CiBitVector   ror           (CiBitVector lhs, const int pos);
  CiBitVector   operator  <<  (CiBitVector lhs, const int pos);
  CiBitVector   operator  >>  (CiBitVector lhs, const int pos);

  /**
   * @name Stream input/output operators
   * @{
   */
  std::istream& operator>>(std::istream&, CiBitVector&);
  std::ostream& operator<<(std::ostream&, CiBitVector&);
  /**
   * @}
   */
}

#endif
