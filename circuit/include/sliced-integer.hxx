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

#include <ci_bit.hxx>
#include <ci_int.hxx>
#include <ci_fncs.hxx>

#include <vector>

using namespace cingulata;

typedef CiBit BitType;

template<typename IntType>
class SlicedInteger : public CiInt
{
public:
  /*! \brief type represented by this class */
  typedef IntType int_t;
  typedef BitType Bit;

public:
  /*! \brief forward constructor */
  SlicedInteger
    (const IntType &value_i = 0) : CiInt(value_i, 8*sizeof(IntType)) {}

  /*! \brief forward constructor */
  SlicedInteger
    (const BitType &value_i) : CiInt(value_i, 1) {}

  /*! \brief forward constructor */
  SlicedInteger
    (const CiInt &value_i) : CiInt(value_i) {}

  SlicedInteger& operator=(const CiInt &other) {
    this->CiInt::operator=(other);
    return *this;
  }
};

template<typename T>
[[deprecated("this function will be removed in next major release")]]
SlicedInteger<T> select(const BitType &cond, const SlicedInteger<T> &a, const SlicedInteger<T> &b) {
  return cingulata::select(cond, a, b);
}

/* Recursive method to compare two integers a  and b defined as arrays of bits (of type BitType) of same size.*/
[[deprecated("this function will be removed in next major release")]]
BitType is_greater(const BitType * a, const BitType *b, unsigned int i, unsigned int j) {
  return cingulata::CiInt(std::vector<BitType>(a+i, a+j)) > cingulata::CiInt(std::vector<BitType>(b+i, b+j));
}

/* Recursive method to verify if two integers a and b, defined as arrays of bits (of type BitType), are equal.*/
[[deprecated("this function will be removed in next major release")]]
BitType is_equal(const BitType * a, const BitType *b, unsigned int i, unsigned int j) {
  return cingulata::CiInt(std::vector<BitType>(a+i, a+j)) == cingulata::CiInt(std::vector<BitType>(b+i, b+j));
}

#endif /* !defined(__SLICED_INTEGER_H__) */
