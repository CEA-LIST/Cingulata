/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
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

/** @file polyring.cxx
 *  @brief Polynomial quotient ring objects
 */

#ifndef __POLY_RING_HXX__
#define __POLY_RING_HXX__

#include <assert.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>
#include <iostream>
#include <vector>
#include <stdint.h>

#include "fhe_params.hxx"

enum rwBase {
	BIN,
	B64,
	B62
};	

/** @brief Polynomial quotient ring class.
 *
 *  @remarks Inner polynomial structure is a \c fmpz_poly_t.
 */
class PolyRing {
private:
  /** @brief Polynomial with coefficients mod N
   */
  fmpz_poly_t polyData;
	static std::string base64_chars;

protected:

  /** @brief Reduce polynomial \poly by the cyclotomic polynomial defining
   *    the polynomial ring
   *
   *  @param poly the polynomial to reduce
   */
  static void reduce(PolyRing &poly);

public:
  /** @brief Build an empty polynomial
   */
  PolyRing();

  /** @brief Build a polynomial ring element from flint polynomial
   *
   *  Build a polynomial ring element of degree \c FheParams::D with
   *    coefficients from polynomial \c polyData.
   *
   *  @param polyData the flint polynomial
   *  @param copy full-copy the list of coefficients when true (default)
   *                and assign-copy when false
   */
  PolyRing(fmpz_poly_t poly, bool copy = true);

  /**
   * @brief Build a polynomial ring element from a vector of uint coefficients
   *
   * @param poly_coeff coefficient vector
   */
  PolyRing(const std::vector<unsigned int>& poly_coeff);

  /** @brief Copy-construct a polynomial
   */
  PolyRing(const PolyRing &poly);

  /** @brief Destructs polynomial object
   */
  ~PolyRing();

  /** @brief In-place apply modulo operation to polynomial coefficients.
   *
   *  Normalize the coefficients of polynomial \c poly with modulo \c q .
   *  The coefficients of the resulting polynomial
   *    belong to the interval (-q/2;q/2].
   *
   * @param poly polynomial to normalize.
   * @param q the modulo.
   */
  static void modulo(PolyRing &poly, const unsigned int q);

  /** @brief In-place apply modulo operation to polynomial coefficients.
   *
   *  Normalize the coefficients of polynomial \c poly with modulo \c q .
   *  The coefficients of the resulting polynomial
   *    belong to the interval (-q/2;q/2].
   *
   * @param poly polynomial to normalize.
   * @param q the modulo.
   */
  static void modulo(PolyRing &poly, const fmpz_t q);

  /** @brief In-place negate polynomial coefficients.
   *
   *  Negate the coefficients of polynomial \c poly .
   *
   * @param poly polynomial to negate.
   */
  static void negate(PolyRing &poly);

  /** @brief In-place add two polynomials.
   *
   *  Add polynomial \c right_poly with polynomial \c left_poly and
   *    store the obtained result in \c left_poly.
   *  This function performs the following operation:
   *    \c left_poly = \c left_poly + \c right_poly
   *
   *  @param left_poly left side of addition, store result here.
   *  @param right_poly right side of addition.
   */
  static void add(PolyRing &left_poly, const PolyRing &right_poly);

  /** @brief In-place subtract two polynomials.
   *
   *  Substract polynomial \c right from the polynomial \c left and
   *    store the obtained result in \c left.
   *  This function performs the following operation:
   *    \c left = \c left - \c right
   *
   *  @param left left side of subtraction (result stored here).
   *  @param right right side of subtraction.
   */
  static void sub(PolyRing &left, const PolyRing &right);

  /** @brief In-place multiply two polynomials.
   *
   *  Multiply polynomial \c right_poly with polynomial \c left_poly and
   *    store the obtained result in \c prod_poly.
   *  This function performs the following operation:
   *    \c prod_poly = ( \c left_poly * \c right_poly )
   *
   *  @param prod_poly result of multiplication.
   *  @param left_poly left side of multiplication.
   *  @param right_poly right side of multiplication.
   */
  static void multiply(PolyRing &prod_poly, const PolyRing &left_poly, const PolyRing &right_poly);

  /** @brief In-place multiply two polynomials.
   *
   *  Multiply polynomial \c right_poly with polynomial \c left_poly and
   *    store the obtained result in \c left_poly.
   *  This function performs the following operation:
   *    \c left_poly = ( \c left_poly * \c right_poly )
   *
   *  @param left_poly left side of multiplication, store result here.
   *  @param right_poly right side of multiplication.
   */
  static void multiply(PolyRing &left_poly, const PolyRing &right_poly);

  /** @brief In-place multiply each polynomial coefficient by a scalar.
   *
   *  Multiply polynomial \c poly with a scalar \c t and
   *    store the obtained result in \c poly.
   *  This function performs the following operation:
   *    \c{poly = poly . t}
   *
   *  @param poly polynomial to multiply, store result here.
   *  @param t scalar.
   */
  static void multiply(PolyRing &poly, const fmpz_t t);

  /** @brief In-place multiply each polynomial coefficient with a rational and
   *    round the result.
   *
   *  Multiply polynomial \c left_poly with rational \c t/q,
   *    round the coefficients of the resulting polynomial
   *    and store the obtained result in \c left_poly.
   *  This function performs the following operation:
   *    \c{left_poly = round(left_poly * t/q)}
   *
   *  @param left_poly polynomial to multiply to.
   *  @param t numerator of the rational.
   *  @param q denominator of the rational.
   */
  static void multiply_round(PolyRing &poly, const unsigned int t, const fmpz_t q);

  /** @brief In-place square a polynomial.
   *
   *  Multiply polynomial \c poly with itself and
   *    store the obtained result in \c poly.
   *  This function performs the following operation:
   *    \c{poly = poly ^ 2}
   *
   *  @param poly polynomial to square
   */
  static void square(PolyRing &poly);

  /** @brief Assignment operator
   *
   *  Assigns a copy of polynomial object \c poly to current object.
   *
   *  @param poly polynomial object to assign to current object
   */
  PolyRing& operator=(const PolyRing &poly);

  /**
   * @brief Sets polynomial coefficient with a \c fmpz_t
   */
  void setCoeff(const unsigned int idx, const fmpz_t value) {
    fmpz_poly_set_coeff_fmpz(this->polyData, idx, value);
  }

  /**
   * @brief Gets polynomial coefficient as a \c fmpz_t
   */
  fmpz* getCoeff(const unsigned int idx) const {
    assert(idx < length());
    return fmpz_poly_get_coeff_ptr(this->polyData, idx);
  }

  /**
   * @brief Sets polynomial coefficient with an \c unsigned
   */
  void setCoeffUi(const unsigned int idx, const unsigned int value) {
    fmpz_poly_set_coeff_ui(this->polyData, idx, value);
  }

  /**
   * @brief Gets polynomial coefficient as an unsigned
   */
  unsigned int getCoeffUi(const unsigned int idx) const {
    assert(idx < length());
    return fmpz_poly_get_coeff_ui(this->polyData, idx);
  }

  /** @brief Read polynomial from an input stream (binary format)
   *
   *  @param in_stream FILE pointer from which to read
   */
  void read(FILE* const in_stream, const rwBase binary = BIN);

  /** @brief Write polynomial to an output stream (binary format)
   *
   *  @param out_stream FILE pointer to which to write
   */
  void write(FILE* const out_stream, const rwBase binary = BIN);

  /** @brief Write printer-friendly version of polynomial to an output stream
   *
   *  @param out_stream stream to which to write
   */
  void print(FILE* const stream) const;

  /** @brief Return the number of coefficient in polynomial
   */
  unsigned int length() const;

  /**
   * @brief write an fmpz to a file
   * 
   * @param d fmpz number to write
   * @param stream file descriptor to write to
   * @param binary write in binary or string form
   */
  static void write_fmpz(FILE* const stream, fmpz_t d, const rwBase binary);
  
  /**
   * @brief read an fmpz from a file
   * 
   * @param d fmpz number to read
   * @param stream file descriptor to read from
   * @param binary read in binary or string form
   */
  static void read_fmpz(fmpz_t d, FILE* const stream, const rwBase binary);

	/** 
	 * @brief Build a polynomial ring element from fmpz_poly_t string
	 * @param prStr corresponds to an fmpz_poly_t 
   */
  PolyRing(const char* prStr);

	/**
	 * @brief convert a random string into a string encoded in Base64
	 * @param in is the string to be encoded in base64
	 */
	static std::string encodeInBase64(const std::string& in);

	/**
	 * @brief decode a string encoded in Base64
	 * @param in is the string to be decoded
	 */
	static std::string decodeFromBase64(const std::string& in);

	/**
	 * @brief encode fmpz_poly_t element in a Base64 string
	 */
	static std::string encodeInBase64(const fmpz_poly_t& p);

	/**
	 * @brief get fmpz_poly_t element from a Base64 string
	 */
	static void decodeFromBase64(const std::string& s, fmpz_poly_t& p);

	/**
	 * @brief encode a polynomial ring element in a Base64 string
	 */
	static std::string encodeInBase64(const PolyRing& prElem);

	/**
	 * @brief recover a polynomial ring element from a Base64 string
	 */
	static void decodeFromBase64(const std::string& s, PolyRing& prElem);
 	
	/** @brief write a PolyRing element to a file stream
 	*/
	static void writeInBase64(FILE* const stream, PolyRing& prElem);

	/** @brief read a PolyRinr element from a file stream
 	*/
	static void readFromBase64(FILE* const stream, PolyRing& prElem);

};

#endif
