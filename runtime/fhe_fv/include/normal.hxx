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

/** @file normal.hxx
 *  @brief Normal distribution pseudo-random number generator.
 */

#ifndef __NORMAL_HXX__
#define __NORMAL_HXX__

#include <flint/fmpz.h>
#include <mpfr.h>

class NormalRng {
  protected:
    /** @brief Variable holds the random state of the PRNG from \c gmp library.
    */
    static gmp_randstate_t randstate;

    /** @brief Initialize PRNG state.
     *
     *  This method initializes \c gmp PRNG state and
     *    seeds the state with a random number.
     *  It should be called before function \c sampleNormal is used.
     */
    static void init();
 
    /** @brief Automatic initializer
     */
    static class _init {
      public:
        inline _init();
        inline ~_init();
    } _initializer;
    
    /** @brief Sample \c mpfr_num according to normal distribution.
     *
     *  Sample \c mpfr_num according to normal distribution with mean \c 0 and
     *    standard deviation \c mpfr_sigma.
     *  The precision (number of digits) if given by the precision of \c mpfr_num.
     *
     *  \todo Reimplement this method to use the uniform RNG defined
     *    in header uniform_rng.h
     *
     *  @param mpfr_num sampled number
     *  @param mpfr_sigma standard deviation
     */
    static void sample(mpfr_t mpfr_num, mpfr_t mpfr_sigma);
     
  public:
    /** @brief Sample \c num according to a normal distribution.
     *
     *  This method samples a number \c num according to a normal distribution.
     *  Variable \c num should be initialized.
     *
     *  @param num sampled number
     *  @param sigma the standard deviation
     *  @param B the distribution interval (~10.sigma)
     */
    static void sample(fmpz_t num, const fmpz_t sigma, const fmpz_t B);
};

#endif
