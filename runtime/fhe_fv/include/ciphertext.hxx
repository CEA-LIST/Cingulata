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

/** @file ciphertext.hxx
 *  @brief Ciphertext class
 */

#ifndef __CIPHERTEXT_HXX__
#define __CIPHERTEXT_HXX__

#include "keys_share.hxx"
#include "polyring.hxx"

#include <assert.h>
#include <string>
#include <vector>
#include <flint/fmpz.h>

class CipherText {
private:
    bool polysAllocated;
    std::vector<PolyRing*> dataPoly;

protected:

  /** @brief In-place multiply a ciphertext with a polynomial.
   *
   *  This function multiplies ciphertext \c ct1 with polynomial
   *    \c p2 and stores the obtained result in \c ct1.
   *
   *  @param ct1 ciphertext to multiply to.
   *  @param p2  polynomial to multiply.
   */
  static void multiply_by_poly(CipherText& ct1, const PolyRing& p2);


public:

  /** @brief Relinearize ciphertext.
   *
   *  This function relinearizes in-place a degree two ciphertext \c ctr.
   *
   *  @param ctr input/output ciphertext.
   *  @param EvalKey Evaluation key.
   */
  static void relinearize(CipherText& ctr, const CipherText& EvalKey);

  /** @brief In-place apply PolyRing::modulo operation to each ciphertext polynomial 
   *
   *  Normalize each polynomial of ciphertext \c ctr with modulo \c q .
   *
   * @param ctr ciphertext to normalize.
   * @param q the modulo.
   */
  static void modulo(CipherText &ctr, const fmpz_t q);

  /** @brief In-place multiply each ciphertext polynomial with a rational and
   *    round the result.
   *
   *  Call PolyRing::multiply_round method on each ciphertext polynomial
   * 
   *  @param ctr ciphertext to multiply to.
   *  @param t numerator of the rational.
   *  @param t denominator of the rational.
   */
  static void multiply_round(CipherText &ctr, const unsigned int t, const fmpz_t q);

  /** @brief In-place multiply two ciphertexts component-wise.
   *
   *  @param left_ctr left side of component-wise multiplication, store result here
   *  @param right_ctr right side of component-wise multiplication
   */
  static void multiply_comp(CipherText &left_ctr, const CipherText &right_ctr);

  /** @brief Access ciphertext polynomials
   */    
  PolyRing& operator[](const unsigned int idx) const {
    assert(idx < size());
    return *dataPoly[idx];
  };

  /** @brief Return number of polynomials in the ciphertext
   */    
  unsigned int size() const {
    return dataPoly.size();
  }

  /** @brief Resize the number of polynomials in the ciphertext
   */    
  void resize(const int newSize);
    
  /** @brief Constructs an empty CipherText.
   */
  CipherText(unsigned int p_nrPolys = 2);
  
  /** @brief Copy-constructs a CipherText object.
   */
  CipherText(const CipherText &ct);
  
  /** @brief Constructs a CipherText object from a polynomial copy.
   */
  CipherText(const PolyRing &cp0);

  /** @brief Constructs a CipherText object from copies of two polynomials.
   */
  CipherText(const PolyRing &cp0, const PolyRing &cp1);
  
  /** @brief Constructs a CipherText object from two polynomials.
   *
   *  \remarks No copies of polynomials \c cp0 and \c cp1 are made!
   */
  CipherText(PolyRing* const cp0, PolyRing* const cp1);
    
  /** @brief Destructs an CipherText object.
   */
  ~CipherText();
  
  /** @brief In-place add two ciphertexts.
   *
   *  Add ciphertext \c ct2 with ciphertext \c ct1 and
   *    store the obtained result in \c ct1.
   *
     *  @param ct1 ciphertext to add to.
     *  @param ct2 ciphertext to add.
   */
  static void add(CipherText &ct1, const CipherText& ct2);
  
  /** @brief In-place subtract two ciphertexts.
   *
   *  Substract ciphertext \c ct2 from ciphertext \c ct1 and
   *    store the obtained result in \c ct1.
   *
     *  @param ct1 ciphertext to subtract from.
     *  @param ct2 ciphertext to subtract.
   */
  static void sub(CipherText &ct1, const CipherText& ct2);

  /** @brief In-place multiply two ciphertexts.
   *
   *  This function multiplies ciphertext \c ct2 with ciphertext
   *    \c ct1 and store the obtained result in \c ct1.
   *
   *  @param ct1 ciphertext to multiply to.
   *  @param ct2 ciphertext to multiply.
   *  @param EvalKey Evaluation key used for relinearization.
   */
  static void multiply(CipherText &ct1, const CipherText& ct2, const CipherText& EvalKey);

  /** @brief In-place multiply two ciphertexts without relinearizing the result.
   *
   *  This function multiplies ciphertext \c ct2 with ciphertext
   *    \c ct1 and store the obtained result in \c ct1. Ciphertext
   *    ct1 is not relinearized.
   *
   *  @param ct1 ciphertext to multiply to.
   *  @param ct2 ciphertext to multiply.
   */
  static void multiply(CipherText &ct1, const CipherText& ct2);

  /** @brief Read ciphertext from an input stream
   *
   *  @param in_stream FILE pointer from which to read
   */
  void read(FILE* const in_stream, const bool binary = true);

  /** @brief Read ciphertext from a file
   *
   *  @param inFileName name of the file from which to read
   */
  void read(const std::string& inFileName, const bool binary = true);
  
  /** @brief Write ciphertext to an output stream
   *
   *  @param out_stream FILE pointer to which to write
   */
  void write(FILE* const out_stream, const bool binary = true) const;    
  
  /** @brief Write ciphertext to a file
   *
   *  @param outFileName file name to which to write
   */
  void write(const std::string& outFileName, const bool binary = true) const;
};

#endif
