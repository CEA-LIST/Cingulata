/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team (formerly Armadillo team)

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

#if !defined(__SLICED_INTEGER_H__)
#define __SLICED_INTEGER_H__

/* compiler includes */
#include <stdint.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <math.h>

/*! \brief integer class to operate on signed integers with all operations
 *  performed in a bitwise fashion.
 *  \tparam BitType bit type used in expression of bitwise operations
 *  \tparam IntType standard integer base type represented by the class (which
 *  defines the size of the sliced integers in bits)
 *
 *  all operators have the same semantic as for the represented base type. */
template <typename BitType, typename IntType>
class SlicedInteger
{
  public:
    /*! \brief type represented by this class */
    typedef IntType int_t;
    typedef BitType Bit;

  private:
    /*! \brief in-place addition of a sliced integer to a shifted operand
     *  \param[in] left_io normal operand of addition.
     *  \param[in] right_i shifted operand of addition.
     *  \param[in] shift_i shift to apply to right operand.
     *
     *  this function performs the operation:
     *  \p left_io = \p left_io + (\p right_i << \p shift_i) */
    static void addition
      (SlicedInteger &left_io,
       const SlicedInteger &right_i,
       const unsigned &shift_i = 0);

    /*! \brief multiplication of a sliced integer by a bit extracted from
     *  another.
     *  \param[in] multiplicand_i left operand of multiplication.
     *  \param[in] multiplier_i right operand of multiplication.
     *  \param[in] index_i bit to extract from multiplier to perform
     *  multiplication.
     *  \return the result of the specified multiplication. */
    static SlicedInteger multiplication
      (const SlicedInteger &multiplicand_i,
       const SlicedInteger &multiplier_i,
       const unsigned &index_i = 0);

  public:
    /*! \brief instantiate a sliced integer with given value.
     *  \param[in] value_i initial value of the integer (default: 0) */
    SlicedInteger
      (const IntType &value_i = 0);
      
    /*! \brief instantiate a sliced integer with a given BitType value. */
    SlicedInteger
      (const BitType &value_i);

    /*! \brief mark the destructor as virtual as the class has virtual functions. */
    virtual ~SlicedInteger() {} 

    /*! \brief access count of bits in the sliced integer. */
    unsigned size(void) const { return 8*sizeof(IntType); }

    /*! \brief access a read-only bit in the word from its index
     *  \param[in] index_i index of accessed bit, must be lesser than the
     *  count of bits in the standard integer type */
    const BitType& operator[]
      (const unsigned &index_i) const;

    /*! \brief access a bit in the word from its index
     *  \param[in] index_i index of accessed bit, must be lesser than the
     *  count of bits in the standard integer type */
    BitType& operator[]
      (const unsigned &index_i);

    /*! \brief access read-only the most significative bit in the word
   */
    const BitType& msb() const;

    /*! \brief access the most significative bit in the word
   */
    BitType& msb();

    /*! \brief access read-only the least significative bit in the word
   */
    const BitType& lsb() const;

    /*! \brief access the least significative bit in the word
   */
    BitType& lsb();

    /*! \brief dump value by writing bit values successively (MSB fisrt) to the
     *  given output stream  */
    virtual void write
      (std::ostream &out_io) const;

    /*! \brief set value from reading bit values successively (MSB fisrt) from the
     *  given input stream  */
    virtual void read
      (std::istream &in_io);

    /*! \brief aggregate bit values to provide value as base type */

    SlicedInteger operator-(void) const;
    SlicedInteger operator~(void) const;

    SlicedInteger operator-(const SlicedInteger &) const;
    SlicedInteger operator+(const SlicedInteger &) const;
#if 0 // ? 
    SlicedInteger operator+(const IntType &) const;    
    SlicedInteger operator+(const BitType &) const;
    SlicedInteger operator-(const IntType &) const;    
    SlicedInteger operator-(const BitType &) const;
    // TODO: Same thing for comparision, in order to remove boolean stuff ?
#endif
    SlicedInteger operator*(const SlicedInteger &) const;
    SlicedInteger operator*(const IntType &) const;
    SlicedInteger operator*(const BitType &) const;

    BitType operator<(const SlicedInteger &) const;
    BitType operator<=(const SlicedInteger &) const;
    BitType operator>(const SlicedInteger &) const;
    BitType operator>=(const SlicedInteger &) const;
    BitType operator==(const SlicedInteger &) const;
    BitType operator!=(const SlicedInteger &) const;
    
    SlicedInteger operator!(void) const;

    SlicedInteger operator|(const SlicedInteger &) const;
    SlicedInteger operator&(const SlicedInteger &) const;
    SlicedInteger operator&(const BitType &) const;
    SlicedInteger operator^(const SlicedInteger &) const;
    SlicedInteger operator<<(const unsigned &) const;
    SlicedInteger operator>>(const unsigned &) const;

    SlicedInteger& operator-=(const SlicedInteger &);
    SlicedInteger& operator+=(const SlicedInteger &);
    SlicedInteger& operator*=(const SlicedInteger &);
    SlicedInteger& operator*=(const IntType &);
    SlicedInteger& operator*=(const BitType &);
    SlicedInteger& operator|=(const SlicedInteger &);
    SlicedInteger& operator&=(const SlicedInteger &);
    SlicedInteger& operator^=(const SlicedInteger &);
    SlicedInteger& operator<<=(const unsigned &);
    SlicedInteger& operator>>=(const unsigned &);
    /*! \brief assignment of a BitType to the least significant bit of a SlidedInteger type
     */
    SlicedInteger& operator=(const BitType &);
    /*! \brief assignment of a SlicedInteger to another SlidedInteger
     */
    SlicedInteger& operator=(const SlicedInteger &);
     /*! \brief assignment of a IntType to a SlidedInteger
     */
    SlicedInteger& operator=(const IntType &value_i);
    
  private:
    /*! \brief current value of the integer, array is sorted by increasing
     *  bit significance. */
    BitType digits_m[8*sizeof(IntType)];
    /*! \brief true if current value is the result of a boolean logic
     *  operator (such as < or >) */
    bool boolean_m;

/* TODO : remove "buggy" code */
  public:
    /*! \brief return true if the integer value is the result of a boolean
     *  logic operator (such as < or >) */
    const bool& boolean(void) const { return boolean_m; }

    /*! \brief set member telling if the integer value is the result of a
     *  boolean logic operator (such as < or >) */
    void boolean(const bool &boolean_i) { boolean_m = boolean_i; }
};


/*! \brief multiplication of an integer of represented base type by a sliced
 *  integer (implementation of commutativity) */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> operator*
  (const IntType &left_i,
   const SlicedInteger<BitType,IntType> &right_i)
{
  /* invert operands to invoke the operator defined in sliced integer class */
  return right_i * left_i;
}

/*! \brief multiplication of a bit by a sliced
 *  integer (implementation of commutativity) */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> operator*
  (const BitType &left_i,
   const SlicedInteger<BitType,IntType> &right_i)
{
  /* invert operands to invoke the operator defined in sliced integer class */
  return right_i * left_i;
}

/*! \brief implementation of the left rotation for sliced integers */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> rol
  (const SlicedInteger<BitType,IntType> &value_i,
   const unsigned &rotation_i)
{
  /* result of rotation operation */
  SlicedInteger<BitType, IntType> result_v;

  for (unsigned index_v=0; index_v < value_i.size(); ++index_v)
  { /* reorganize bits in resulting word */
    result_v[(index_v+rotation_i)%value_i.size()] = value_i[index_v];
  }

  return result_v;
}

/*! \brief implementation of the right rotation for sliced integers */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> ror
  (const SlicedInteger<BitType,IntType> &value_i,
   const unsigned &rotation_i)
{
  /* result of rotation operation */
  SlicedInteger<BitType, IntType> result_v;

  for (unsigned index_v=0; index_v < value_i.size(); ++index_v)
  { /* reorganize bits in resulting word  */
    result_v[index_v] = value_i[(index_v+rotation_i)%value_i.size()];
  }

  return result_v;
}

#if 0
/*! \brief generic implementation of bit slice extraction */
template <typename BitType, typename IntType, typename OtherType>
void field
  (SlicedInteger<BitType,OtherType> &field_o,
   const SlicedInteger<BitType,IntType> &value_i,
   const unsigned &low_i,
   const unsigned &high_i)
{
  /* interface contract */
  assert (high_i < value_i.size());
  assert (low_i <= high_i);
  assert (high_i-low_i+1 <= field_o.size());
  
  for (unsigned i=low_i; i<=high_i; ++i)
  { /* copy requested bits to result */
    field_o[i-low_i] = value_i[i];
  }
  
  for (unsigned i=high_i+1; i<field_o.size(); ++i)
  { /* reset the other bits */
    field_o[i-low_i] = 0;
  }
}
#endif

/*! \brief implementation of the formatted read operation */
template<typename BitType, typename IntType>
std::istream& operator>>
  (std::istream &in_io,
   SlicedInteger<BitType,IntType> &value_o)
{
  value_o.read (in_io);
  return in_io;
}

/*! \brief implementation of the formatted write operation */
template<typename BitType, typename IntType>
std::ostream& operator<<
  (std::ostream &out_io,
   const SlicedInteger<BitType,IntType> &value_i)
{
  value_i.write (out_io);
  return out_io;
}

/*! \brief implementation of the formatted read operation */
template<typename BitType>
std::istream& operator>>
  (std::istream &in_io,
   BitType &value_o)
{
  value_o.read (in_io);
  return in_io;
}

/*! \brief implementation of the formatted write operation */
template<typename BitType>
std::ostream& operator<<
  (std::ostream &out_io,
   const BitType &value_i)
{
  value_i.write (out_io);
  return out_io;
}

/*! \brief implementation of the ternary operator for bitwise words */
template<typename BitType, typename IntType>
SlicedInteger<BitType,IntType> select
 (const BitType &selector_i,
  const SlicedInteger<BitType,IntType> &true_i,
  const SlicedInteger<BitType,IntType> &false_i)
{
  SlicedInteger<BitType,IntType> result = (selector_i * true_i) ^ ((!selector_i) * false_i);
  result.boolean(true_i.boolean() and false_i.boolean());
  return result;
}

/* see declaration in class definition */
template <typename BitType, typename IntType>
void SlicedInteger<BitType,IntType>::addition
  (SlicedInteger &left_io,
   const SlicedInteger &right_i,
   const unsigned &first_i)
{
  /* carry flag for current 1-bit addition */
  BitType carry_v;

  for (unsigned index_v=first_i; index_v < left_io.size(); ++index_v)
  { /* optimized addition adding only useful bits */
    /* full 1-bit addition taking carry into account */
    BitType xor_v1 = left_io[index_v] ^ carry_v;

    if (index_v < left_io.size()-1)
    { /* case: must update the carry */
      BitType xor_v2 = right_i[index_v] ^ carry_v;
      carry_v = (xor_v1 & xor_v2) ^ carry_v;
    }

    /* update result of in-place addition */
    left_io[index_v] = right_i[index_v] ^ xor_v1;
  }
}


/* see declaration in class definition */
/* #if 0
   Méthode redondante avec mul par BitType à supprimer après refactoring boolean.
*/
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::multiplication
  (const SlicedInteger &multiplicand_i,
   const SlicedInteger &multiplier_i,
   const unsigned &index_i)
{
  /* value of extracted bit */
  const BitType bit_v = multiplier_i[index_i];
  /* product of multiplicand by the extracted bit */
  SlicedInteger product_v;

  for (unsigned index_v=index_i; index_v < multiplicand_i.size(); ++index_v)
  { 
    product_v[index_v] = multiplicand_i[index_v-index_i] & bit_v;
  }

  return product_v;
}

/* see declaration in class definition */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>::SlicedInteger
  (const IntType &value_i)
{
  /* interface contract */
  assert (this->size() != 0);

  assign(*this, value_i);
}


/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>::SlicedInteger
  (const BitType &value_i)
 : boolean_m(true)
{
  /* interface contract */
  assert (this->size() != 0);

  (*this)[0]=value_i;
  
  for (int index_v = this->size() - 1; index_v > 0; --index_v)
  {
    (*this)[index_v]=BitType(false);
  }
}

/* see declaration above */
template <typename BitType, typename IntType>
const BitType& SlicedInteger<BitType,IntType>::operator[]
  (const unsigned &index_i) const
{
  /* interface contract */
  assert (index_i < this->size());
  return digits_m[index_i];
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType& SlicedInteger<BitType,IntType>::operator[]
  (const unsigned &index_i)
{
  const SlicedInteger &this_v = const_cast<const SlicedInteger&>(*this);
  return const_cast<BitType&>(this_v[index_i]);
}

/* see declaration above */
template <typename BitType, typename IntType>
const BitType& SlicedInteger<BitType,IntType>::msb() const {
  return (*this)[this->size() - 1];
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType& SlicedInteger<BitType,IntType>::msb() {
  return (*this)[this->size() - 1];
}

/* see declaration above */
template <typename BitType, typename IntType>
const BitType& SlicedInteger<BitType,IntType>::lsb() const {
  return (*this)[0];
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType& SlicedInteger<BitType,IntType>::lsb() {
  return (*this)[0];
}

/* see declaration above */
template <typename BitType, typename IntType>
void SlicedInteger<BitType,IntType>::write
  (std::ostream &out_io) const
{
  for (int index_v = this->size() - 1; index_v >= 0; --index_v)
  { /* trigger the write behavior for all bit values */
    (*this)[index_v].write(out_io);
  }
}

/* see declaration above */
template <typename BitType, typename IntType>
void SlicedInteger<BitType,IntType>::read
  (std::istream &in_io)
{
  for (int index_v = this->size() - 1; index_v >= 0; --index_v)
  { /* trigger the read behavior for all bit values */
    (*this)[index_v].read(in_io);
  }
  boolean(false);
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator-
  (void) const
{
  /* compute one's complement of this value */
  SlicedInteger complement_v = ~(*this); /*warning ! have to test it because of bug in subtraction*/
  /* create a sliced integer with the value 1 to get two's complement */
  SlicedInteger one_v(1);
  one_v.boolean (true);

  /* return two's complement of this value */
  return complement_v += one_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator~
  (void) const
{
  /* one's complement of this value */
  SlicedInteger complement_v;
  complement_v.boolean(false);

  for (unsigned index_v=0; index_v < this->size(); ++index_v)
  { /* compute one's complement of this value */
    complement_v[index_v] = !(*this)[index_v];
  }

  return complement_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator-
  (const SlicedInteger &operand_i) const
{
  /* use addition and negation methods to compute the difference */
  return (*this) + -operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator+
  (const SlicedInteger &operand_i) const
{
  /* sum of this value and the operand */
  SlicedInteger sum_v;
  sum_v.boolean(false);

  /* carry flag for addition */
  BitType carry_v;

  if (this->boolean() && !operand_i.boolean())
  { /* case: this integer is a boolean value, can minimize carry overhead */
    /* invert operand order and benefit from optimized case */
    sum_v = operand_i + (*this);
  }
  else if (operand_i.boolean())
  { 
    /* perform 1-bit addition and get carry to begin with */
    sum_v[0] = (*this)[0] ^ operand_i[0];
    carry_v = (*this)[0] & operand_i[0];

    /* case: the operand is a boolean value, can optimize carry overhead */
    /* decide which optimization to apply */
    if (this->boolean())
    { /* case: both operands are boolean values */
      /* finalize 1-bit addition with carry propagation */
      sum_v[1] = carry_v;
    }
    else
    { /* case: only one operand is a boolean value */
      for (unsigned index_v=1; index_v < this->size(); ++index_v)
      { /* perform addition knowing that an operand's bits are all 0 */
        /* simplification of the addition ignoring XOR of inputs */
        sum_v[index_v] = (*this)[index_v] ^ carry_v;

        if (index_v < this->size()-1)
        { /* case: must update the carry */
          /* simplification of the carry update ignoring initial AND
           * of inputs and final XOR */
          carry_v = (*this)[index_v] & carry_v;
        }
      }
    }
  }
  else
  { /* case: neither inputs are boolean values */
    sum_v = *this;
    SlicedInteger::addition (sum_v, operand_i);
  }

  return sum_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator*
  (const SlicedInteger &operand_i) const
{
  /* product of this value and operand */
  SlicedInteger product_v;
  product_v.boolean(false);

  if (this->boolean() && !operand_i.boolean())
  { /* case: this integer is a boolean value, can convert operation to some
     * form of bitwise AND */
    /* invert operand order and benefit from optimized case */
    return operand_i * (*this);
  }
  else if (operand_i.boolean())
  { /* case: this integer is a boolean value, can convert operation to a
     * bitwise AND */
    if(this->boolean())
    { /* case: both operands are boolean values */
      /* product is a boolean value */
      product_v[0] = (*this)[0] & operand_i[0];
      product_v.boolean (true);
    }
    else
    { /* case: only one operand is a boolean value */
      product_v = SlicedInteger::multiplication (*this, operand_i);
    }
  }
  else
  { /* case: neither operand is a boolean value */
    for (unsigned index_v=0; index_v < this->size(); ++index_v)
    { /* accumulate the result of the boolean multiplications of value by
       * each bit in operand */
      /* product of this value by current operand's bit */
      SlicedInteger result_v =
        SlicedInteger::multiplication (*this, operand_i, index_v);

      /* accumulate intermediate result into final product */
      SlicedInteger::addition (product_v, result_v, index_v);
    }
  }

  return product_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator*
  (const IntType &operand_i) const
{
  /* product of operands and intermediate result of multiplication of this
   * value's bits by one bit from the operand */
  SlicedInteger product_v;

  for (unsigned step_v=0,bit_v=1; step_v < this->size(); ++step_v,bit_v<<=1)
  { /* accumulate the result of the boolean multiplications of value by
     * each bit in operand */
    if (operand_i & bit_v)
    { /* case: multipication of value by current bit of operand is not 0 */
      /* product of this value by current operand's bit (computed with an
       * optimization of multiplication by a known power of 2) */
      SlicedInteger result_v = (*this) << step_v;

      /* accumulate intermediate result into final product */
      SlicedInteger::addition (product_v, result_v, step_v);
    }
  }

  return product_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator*
  (const BitType &operand_i) const
{
  SlicedInteger product_v;
  product_v.boolean(this->boolean());

  for (uint i = 0; i < this->size(); ++i) {
    product_v[i] = (*this)[i] * operand_i;
  }

  return product_v;
}


/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator<
  (const SlicedInteger &operand_i) const
{
  /* the least significant bit is 1 if this value is lesser than the other */
  BitType lesser_v;

  /* optimized method with circuit depth log(l)+1*/
  lesser_v  =!(is_greater((*this).digits_m, operand_i.digits_m, 0, 8*sizeof(IntType)) ^ is_equal((*this).digits_m, operand_i.digits_m, 0, 8*sizeof(IntType)) );
    
  return lesser_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator<=
  (const SlicedInteger &operand_i) const
{
  /* the least significant bit is 1 if this value is lesser than or equals to the other */
  BitType lesser_eq_v;

  /* optimized method with circuit depth log(l)+1*/
  lesser_eq_v = !is_greater((*this).digits_m, operand_i.digits_m, 0, 8*sizeof(IntType));
  return lesser_eq_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator>
  (const SlicedInteger &operand_i) const
{
  /* the least significant bit is 1 if this value is greater than the other */
  BitType greater_v;
  
  /* optimized method with circuit depth log(l)+1*/
  greater_v = is_greater((*this).digits_m, operand_i.digits_m, 0, 8*sizeof(IntType));
    
  return greater_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator>=
  (const SlicedInteger &operand_i) const
{
  /* the least significant bit is 1 if this value is greater than or equals to the other */
  BitType greater_eq_v;

  /* optimized method with circuit depth log(l)+1*/
  greater_eq_v = !is_greater(operand_i.digits_m, (*this).digits_m, 0, 8*sizeof(IntType));
  return greater_eq_v;
}


/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator==
  (const SlicedInteger &operand_i) const
{
  /* Optimized method with a corresponding boolean circuit of size equal with 8*sizeof(IntType) and
  multiplicative depth equal with ceil(log(8*sizeof(IntType))) --> but doesn't work :( TODO */
  BitType equal_v;
  
  equal_v = is_equal((*this).digits_m,  operand_i.digits_m, 0, 8*sizeof(IntType));
  return equal_v;
}


/* see declaration above */
template <typename BitType, typename IntType>
BitType SlicedInteger<BitType,IntType>::operator!=
  (const SlicedInteger &operand_i) const
{
  return !(*this == operand_i);
}

/* Recursive method to compare two integers a  and b defined as arrays of bits (of type BitType) of same size.*/
template <typename BitType>
 BitType is_greater(const BitType * a, const BitType *b, unsigned int i, unsigned int j) {
    if(j==1) return a[i] & !b[i];
    
    unsigned int l=floor(j/2);
    BitType result=is_greater(a,b,i+l,j-l) ^ (is_equal(a,b,i+l,j-l) & is_greater(a,b,i,l));
    return result;
}

/* Recursive method to verify if two integers a and b, defined as arrays of bits (of type BitType), are equal.*/
template <typename BitType>
BitType is_equal(const BitType * a, const BitType *b, unsigned int i, unsigned int j) {
  if(j==1) return !a[i]^b[i];
  unsigned int l=floor(j/2);
  
  return is_equal(a,b,i+l,j-l) & is_equal(a,b,i,l);
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator!
  (void) const
{
  /* interface contract */
  assert (this->boolean());

  /* invertion of the least significant bit */
  SlicedInteger not_v;
  not_v[0] = !(*this)[0];
  return not_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator|
  (const SlicedInteger &operand_i) const
{
  /* result of bitwise OR */
  SlicedInteger or_v;

  for (unsigned index_v=0; index_v < this->size(); ++index_v)
  { /* apply OR operation to all bits in operands */
    or_v[index_v] = (*this)[index_v] | operand_i[index_v];
  }
  or_v.boolean(this->boolean() && operand_i.boolean());

  return or_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator&
  (const SlicedInteger &operand_i) const
{
  /* result of bitwise AND */
  SlicedInteger and_v;

  for (unsigned index_v=0; index_v < this->size(); ++index_v)
  { /* apply AND operation to all bits in operands */
    and_v[index_v] = (*this)[index_v] & operand_i[index_v];
  }
  and_v.boolean(this->boolean() && operand_i.boolean());

  return and_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator&
  (const BitType &operand_i) const
{
  /* result of bitwise AND */
  SlicedInteger and_v;

  for (unsigned index_v=0; index_v < this->size(); ++index_v)
  { /* apply AND operation to all bits in operands */
    and_v[index_v] = (*this)[index_v] & operand_i;
  }
  and_v.boolean(this->boolean());

  return and_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator^
  (const SlicedInteger &operand_i) const
{
  /* result of bitwise XOR */
  SlicedInteger xor_v;

  for (unsigned index_v=0; index_v < this->size(); ++index_v)
  { /* apply XOR operation to all bits in operands */
    xor_v[index_v] = (*this)[index_v] ^ operand_i[index_v];
  }
  xor_v.boolean(this->boolean() && operand_i.boolean());

  return xor_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator<<
  (const unsigned &shift_i) const
{
  /* interface contract */
  assert (shift_i < this->size());

  /* result of shift operation */
  SlicedInteger result_v;
  result_v.boolean(false);

  for (unsigned index_v=shift_i; index_v < this->size(); ++index_v)
  { /* assign potentially non-zero bits in resulting word  */
    result_v[index_v] = (*this)[index_v-shift_i];
  }

  return result_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType> SlicedInteger<BitType,IntType>::operator>>
  (const unsigned &shift_i) const
{
  
  /* interface contract */
  assert (shift_i < this->size());

  /* result of shift operation */
  SlicedInteger result_v;
  result_v.boolean(false);

  /* index of currently assigned bit */
  unsigned index_v;

  for (index_v=0; index_v < this->size()-shift_i; ++index_v)
  { /* assign potentially non-zero bits in resulting word  */
    result_v[index_v] = (*this)[index_v+shift_i];
  }

  for (; index_v < this->size(); ++index_v)
  { /* perform sign extension */
    result_v[index_v] = (*this)[this->size()-1];
  }

  return result_v;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator-=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) - operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator+=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) + operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator*=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) * operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator*=
  (const IntType &operand_i)
{
  return (*this) = (*this) * operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator|=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) | operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator&=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) & operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator^=
  (const SlicedInteger &operand_i)
{
  return (*this) = (*this) ^ operand_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator<<=
  (const unsigned &shift_i)
{
  return (*this) = (*this) << shift_i;
}

/* see declaration above */
template <typename BitType, typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator>>=
  (const unsigned &shift_i)
{
  return (*this) = (*this) >> shift_i;
}

/* see declaration above */
template <typename BitType,typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator=(const BitType &lsb)
{
  (*this)[0]=lsb;
  this->boolean(true);
  
  return (*this);
}

/* see declaration above */
template <typename BitType,typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator=(const SlicedInteger &other)
{
  for (unsigned index_v=0; index_v < this->size(); ++index_v) {
    (*this)[index_v]=other[index_v];
  }
  
  this->boolean(other.boolean());

  return (*this);
}

/* see declaration above */
template <typename BitType,typename IntType>
SlicedInteger<BitType,IntType>& SlicedInteger<BitType,IntType>::operator=(const IntType &value_i)
{
  assign(*this,value_i);
  return (*this);
}

/* function for dereferencing an encrypted array and replacing an encrypted value from the array with another 
 * Parameters: the array in std::vector form with each element of type SlicedInteger; n - the size of the array; index-the position of the element to be
 * replaced; x the element to be placed in the array */
template <typename BitType, typename IntType>
void deref(std::vector<SlicedInteger<BitType,IntType> > &arr,
   const int n,
   const SlicedInteger<BitType,IntType> &index, 
   const SlicedInteger<BitType,IntType> &x)
{
  assert(n>0);
  int i;
  
  for(i=0;i<n;i++) {
    SlicedInteger<BitType,IntType> j=SlicedInteger <BitType,IntType> (i);
    SlicedInteger<BitType,IntType> trueIndex=(j==index);
    trueIndex.boolean(true);

    SlicedInteger<BitType,IntType> valueArr=arr[i];
    arr[i]=select(trueIndex,x,valueArr);
  }
  
}

/* function for dereferencing an encrypted array and replacing an encrypted value from the array with another 
 * Parameters: the array in C-form with each element of type SlicedInteger; n - the size of the array; index-the position of the element to be
 * replaced; x the element to be placed in the array */
template <typename BitType, typename IntType>
void deref(SlicedInteger<BitType,IntType>  *arr,
   const int n,
   const SlicedInteger<BitType,IntType> &index, 
   const SlicedInteger<BitType,IntType> &x)
{
  assert(n>0);
  int i;
  
  for(i=0;i<n;i++) {
    SlicedInteger<BitType,IntType> j=SlicedInteger <BitType,IntType> (i);
    SlicedInteger<BitType,IntType> trueIndex;
    trueIndex=(j==index);
    trueIndex.boolean(true);
    SlicedInteger<BitType,IntType> valueArr=arr[i];
    arr[i]=select(trueIndex,x,valueArr);
  }
}

/* Function to assign a integer value of type OtherIntType to an already declared SlicedInteger.
 * Causes compilation error if OtherIntType is not integral (e.g. double or float)*/
template <
  typename BitType, typename IntType, typename OtherIntType,
  typename enable=
  typename std::enable_if<std::is_integral<IntType>::value && std::is_integral<OtherIntType>::value> ::type
>
void assign(SlicedInteger<BitType, IntType> &current, const OtherIntType &other)
{
  IntType current_value = other;

  if ((other == 0) || (other == 1)) {
    current.boolean(true);
  } else {
    current.boolean(false);
  }
  
  for (unsigned index_v=0; index_v < current.size(); ++index_v) {
    /* set all bits at zero */
    current[index_v] = false;
  }
  
  /* update with the new current value */
  if (current_value)
  { /* case: some bits are not zero */
    for (unsigned index_v=0; index_v < current.size(); ++index_v)
    { /* set initial value of each bit */
      if ((current_value>>index_v) & 1)
      { /* case: bit at current index is set in input value */
        current[index_v] = true;
      }
    }
  }
}

/*
 * this assign function means that : current = other
 * */
template <
  typename BitType, typename IntType, typename OtherIntType,
  typename enable=
  typename std::enable_if<std::is_integral<IntType>::value && std::is_integral<OtherIntType>::value>::type
>
void assign(SlicedInteger<BitType, IntType> &current, const SlicedInteger<BitType, OtherIntType> &other)
{
  current.boolean(other.boolean());

  if(other.size()>current.size()) {
   /*copy only the last current.size() bits, including the bit sign from the other parameter*/
    field(current,other,0,current.size()-1);        
  } else {
    /*copy all the values from the other parameter and repeat the bit sign, supposed to be the most significant bit (on the last position), in all the extra bits*/
    field(current, other,0,other.size()-1);
    for(unsigned index_v=other.size(); index_v<current.size(); ++index_v) {
      // current[index_v]=other.msb();
      current[index_v]=BitType(0);
    }
  }    
}

#endif /* !defined(__SLICED_INTEGER_H__) */

