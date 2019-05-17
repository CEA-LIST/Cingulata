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

#ifndef UNIFORM_HXX
#define UNIFORM_HXX

#include <flint/fmpz.h>

class UniformRng {
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

  /**
   * @brief      Sample @c size random bytes
   *
   * @param      buff  buffer where bytes are written
   * @param[in]  size  number of bytes to sample
   */
  static void sample(char *buff, const unsigned size);

  /**
   * @brief      Sample an unsigned integer
   *
   * @return     uniformely distributed unsigned integer
   */
  static unsigned sample();

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
};

#endif
