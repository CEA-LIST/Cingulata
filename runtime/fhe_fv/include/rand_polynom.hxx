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

/** @file rand_polynom.hxx
 *  @brief Random polynomial generator with different distributions.
 */

#ifndef __RAND_POLYNOM_HXX__
#define __RAND_POLYNOM_HXX__

#include <flint/fmpz_poly.h>

class RandPolynom {
  protected:
    /** @brief Set a binary polynomial with coefficients from a given number.
     *
     *  Build a polynomial with binary coefficients. The coefficient values
     *    are taken from number \c num binary form.
     *  The degree of the built polynomial is \c degree.
     *
     *  @param poly the polynomial to set
     *  @param num the number from which to extract coefficients
     */  
    static void populateBinaryPolyRing(fmpz_poly_t poly, fmpz_t num);
  
  public:

    /** @brief Sample a polynomial according to a binary uniform distribution
     *
     *  This method samples a polynomial of length \c len with coefficients
     *    sampled from a binary uniform distribution.
     *
     *  @param poly the polynomial to sample
     *  @param len the length of the polynomial to sample
     */
    static void sampleUniformBinary(fmpz_poly_t poly, unsigned int len);

    /** @brief Sample a polynomial according to a binary uniform distribution
     *    with a given hamming weight (number of unitary coefficients)
     *
     *  This method samples a polynomial of length \c len with coefficients
     *    sampled from a binary uniform distribution with a hamming weight
     *    \c hammingWeight.
     *
     *  @param poly the polynomial to sample
     *  @param len the length of the polynomial to sample
     *  @param hammWeight the hamming weight of the polynomial to sample
     */
    static void sampleUniformBinary(fmpz_poly_t poly, unsigned int len,
                                    unsigned int hammingWeight);

    /** @brief Sample a polynomial according to an uniform distribution
     *
     *  This method samples a polynomial of length \c len with coefficients
     *    of \c coefBitCnt bits sampled from a uniform distribution.
     *  Sampled polynomial coefficients are positive.
     *
     *  @param poly the polynomial to sample
     *  @param len the length of the polynomial to sample
     *  @param coefBitCnt number of bits in polynomial coefficients
     */
    static void sampleUniform(fmpz_poly_t poly, unsigned int len,
                              unsigned int coefBitCnt);

    /** @brief Sample a polynomial according to an uniform distribution
     *
     *  This method samples a polynomial of length \c len with coefficients
     *    sampled from a uniform distribution defined on \c{[0;q)}.
     *  The distribution interval (\c q) must be a power of two.
     *
     *  @param poly the polynomial to sample
     *  @param len the length of the polynomial to sample
     *  @param q uniform distribution interval
     */
    static void sampleUniform(fmpz_poly_t poly, unsigned int len, fmpz_t q);
    
    /** @brief Sample a polynomial according to a normal distribution
     *
     *  This method samples a polynomial of length \c len with coefficients
     *    sampled from a normal distribution with standard deviation \c sigma
     *    and defined on interval \c{[-B;B]}.
     *
     *  @param poly the polynomial to sample
     *  @param len the length of the polynomial to sample
     *  @param sigma the standard deviation
     *  @param B the distribution interval (~10.sigma)
     */
    static void sampleNormal(fmpz_poly_t poly, unsigned int len, fmpz_t sigma, 
                              fmpz_t B);
};

#endif
