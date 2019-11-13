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

#include <assert.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

/** @brief See header for description.
 */
void UniformRng::init() {}

/** @brief See header for description.
 */
UniformRng::_init::_init() { UniformRng::init(); }

/** @brief See header for description.
 */
UniformRng::_init UniformRng::_initializer;

unsigned int bits2Bytes(unsigned int bitSize) {
  double byteSize = ceil((float)bitSize / 8);
  return (int)byteSize;
}

/** @brief See header for description.
 */
void UniformRng::sample(fmpz_t num, const unsigned bitCnt) {
  const unsigned int byteCnt = bits2Bytes(bitCnt);
  char buff[byteCnt];

  sample(buff, byteCnt);

  fmpz_bit_unpack_unsigned(num, (mp_limb_t *)buff, 0, bitCnt);
}

void UniformRng::sample(char *buff, const unsigned size) {
  int randDev = open("/dev/urandom", O_RDONLY);
  if (randDev == -1) {
    cerr << "File: " << __FILE__ << " line: " << __LINE__
         << " - cannot open random generator \"/dev/urandom\"" << endl;
    exit(-1);
  }

  unsigned int r = read(randDev, buff, size);
  assert(r == size);
  close(randDev);
}

unsigned UniformRng::sample() {
  constexpr unsigned size = sizeof(unsigned);
  assert(size > 0);
  char buff[size];
  UniformRng::sample(buff, size);

  unsigned result = 0;
  for (unsigned i = 0; i < size; ++i)
    result |= unsigned(buff[i]) << (8 * i);

  return result;
}
