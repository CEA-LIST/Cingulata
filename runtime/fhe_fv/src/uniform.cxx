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

#include "uniform.hxx"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

using namespace std;

/** @brief See header for description.
 */
void UniformRng::init() {}

/** @brief See header for description.
 */
UniformRng::_init::_init() {
  UniformRng::init();
}

/** @brief See header for description.
 */
UniformRng::_init UniformRng::_initializer;

unsigned int sizeInBits(unsigned int bitCnt) {
  double bitSize = ceil(log2((double)bitCnt + 0.5));
  return (int)bitSize;
}

unsigned int bits2Bytes(unsigned int bitSize) {
  double byteSize = ceil((float)bitSize / 8);
  return (int)byteSize;
}

/** @brief See header for description.
 */
void UniformRng::sample(fmpz_t num, unsigned int bitCnt) {
  int randDev = open("/dev/urandom", O_RDONLY);
  if (randDev == -1) {
    cerr << "File: " << __FILE__ << " line: " << __LINE__
      << " - cannot open random generator \"/dev/urandom\"" << endl;
    exit(-1);
  }

  unsigned int byteCnt = bits2Bytes(bitCnt);
  unsigned char buff[byteCnt];

  unsigned int r = read(randDev, buff, byteCnt);
  assert(r == byteCnt);
  close(randDev);

  fmpz_bit_unpack_unsigned(num, (mp_limb_t*)buff, 0, bitCnt);
}

/** @brief See header for description.
 */
void UniformRng::sample(fmpz_t num_p,
                        unsigned int bitCnt,
                        unsigned int hammingWeight) {
  assert(2 * hammingWeight <= bitCnt);

  int randDev = open("/dev/urandom", O_RDONLY);
  if (randDev == -1) {
    cerr << "File: " << __FILE__ << " line: " << __LINE__
      << " - cannot open random generator \"/dev/urandom\"" << endl;
    exit(-1);
  }

  unsigned int bitSize = sizeInBits(bitCnt - 1);
  assert(bitSize <= FLINT_BITS);
  
  unsigned int byteSize = bits2Bytes(bitSize);
  unsigned int mask = (1 << bitSize) - 1;

  fmpz_zero(num_p);

  while (fmpz_popcnt(num_p) < hammingWeight) {
    unsigned int pos = 0;
    unsigned char b;

    for (unsigned int i = 0; i < byteSize; i++) {
      int r = read(randDev, &b, 1);
      assert(r == 1);
      pos |= (b << (8 * i));
    }
    pos &= mask;

    if (pos < bitCnt) {
      fmpz_combit(num_p, pos);
    }
  }  
}

