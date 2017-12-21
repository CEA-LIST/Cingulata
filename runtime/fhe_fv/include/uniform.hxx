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

/** @file uniform.hxx
 *  @brief Uniform random number generator (uses /dev/urandom).
 */

#ifndef __UNIFORM_HXX__
#define __UNIFORM_HXX__

#include <flint/fmpz.h>

class UniformRng {
  protected:
    /** @brief Initializes uniform RNG.
     */
    static inline void init();

    /** @brief Automatic initializer
     */
    static class _init {
      public:
        inline _init();
    } _initializer;

  public:
    /** @brief Sample number from uniform distribution.
     *
     *  This method samples a number \c num uniformly
     *    defined on interval [0;2^bitCnt).
     *  Variable \c num should be initialized.
     *
     *  @param num sampled number
     *  @param bitCnt number of bits in \c num
     */
    static void sample(fmpz_t num, unsigned int bitCnt);

    /** @brief Sample number from uniform distribution with a given
     *     Hamming weight.
     *
     *  This method samples a number \c num uniformly. The resulting
     *    \c num has a Hamming weight \c hammingWeight, thus \c hammingWeight 
     *    bits of \c num will be logical ones.
     *  The number of bits in \c num is given by \c bitCnt.
     *  Variable \c num should be initialized.
     *
     *  @remarks Runtime of this function depends statistically on
     *    the ratio between \c hammingWeight and \c bitCnt.
     *    For performance reason \c hammingWeight is limited to half of
     *    \c bitCnt.
     *
     *  @param num sampled number
     *  @param bitCnt number of bits in \c num
     *  @param hammingWeight Hamming weight of obtained \c num
     */
    static void sample(fmpz_t num, unsigned int bitCnt,
                        unsigned int hammingWeight);
};

#endif
