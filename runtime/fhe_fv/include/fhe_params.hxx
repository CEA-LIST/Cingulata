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

/** @file fhe_params.hxx
 *  @brief FHE scheme parameters.
 */

#ifndef __FHE_PARAMS_HXX__
#define __FHE_PARAMS_HXX__

#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>

class FheParams {
  public:
    /** @brief Plaintext polynomial coefficient modulo
     */
    static unsigned int T;

    /** @brief Ciphertext polynomial coefficient modulo, q
     */
    static fmpz_t Q;

    /** @brief Ciphertext polynomial coefficient modulo bit-size, q
     */
    static unsigned int Q_bitsize;

    /** @brief floor(Q/T)
     */
    static fmpz_t Delta;

    /** @brief Relinearization key (version 2) coefficient modulo factor, p
     */
    static fmpz_t P;

    /** @brief Relinearization key (version 2) coefficient modulo, p.q
     */
    static fmpz_t PQ;

    /** @brief Relinearization key (version 2) coefficient modulo, p.q
     */
    static unsigned int PQ_bitsize;

    /** @brief Is power of two cyclotomic
     *
     *  Boolean flag for identifying power of two cyclotomic as  modulo of
     *    the quotient ring, i.e.  \Phi_{2^{n}} = x^{2^{n-1}} + 1
     */
    static bool IsPowerOfTwoCyclotomic;

    /** @brief Ciphertext polynomial ring modulo cyclotomic polynomial
     */
    static fmpz_poly_t PolyRingModulo;

    /** @brief Precomputed powers inverse of modulo cyclotomic polynomial
     */
    static fmpz_poly_powers_precomp_t PolyRingModuloInv;

    /** @brief Degree of ciphertext polynomial ring modulo cyclotomic polynomial
     */
    static unsigned int D;

    /** @brief Gaussian noise distribution standard deviation sigma
     */
    static fmpz_t SIGMA;

    /** @brief Gaussian noise distribution bound B
     */
    static fmpz_t B;

    /** @brief Gaussian noise distribution standard deviation
     *    Relinearization version 2
     */
    static fmpz_t SIGMA_K;

    /** @brief Gaussian noise distribution bound B
     *    Relinearization version 2
     */
    static fmpz_t B_K;

    /** @brief Secret key hamming weight
     */
    static unsigned int SK_H;

    /** @brief Polynomial coefficients R/W base
     */
    static unsigned int POLY_RW_BASE;

    /** @brief Read FHE parameters from XML.
     */
    static void readXml(const char* const fileName);

    /** @brief Compute parameters, other than the read one
     */
    static void computeParams();

    /** @brief Verify if polynomial is a power of two cyclotomic
     */
    static bool isPowerOfTwoCyclotomicPolynomial(fmpz_poly_t poly);

  protected:

    /** @brief Hide constructor
     */
    FheParams() {}

    /** @brief Initialize FHE parameters class
     *
     *  A "static" class constructor/destructor
     */
    static class _init {
      public:
        _init();
        ~_init();
    } _initializer;
};

#endif
