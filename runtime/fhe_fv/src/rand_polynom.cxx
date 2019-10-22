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

#include "rand_polynom.hxx"
#include "normal.hxx"
#include "uniform.hxx"

#include <cassert>

/** @brief See header for description.
 */
void RandPolynom::sampleUniformBinary(fmpz_poly_t poly) {
  const unsigned len = fmpz_poly_length(poly);
  const unsigned byte_cnt = (len / 8) + 1;
  char buff[byte_cnt];
  UniformRng::sample(buff, byte_cnt);

  for (unsigned i = 0; i < len; ++i) {
    const unsigned tmp = (buff[i / 8] >> (i % 8)) & 1;
    fmpz_set_ui(poly->coeffs + i, tmp);
  }
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniformBinary(fmpz_poly_t poly,
                                      unsigned int hammingWeight) {
  const unsigned len = fmpz_poly_length(poly);
  assert(2 * hammingWeight <= len);

  unsigned bits_set = 0;
  while (bits_set < hammingWeight) {
    unsigned idx = UniformRng::sample();
    idx %= len;
    if (fmpz_get_ui(poly->coeffs + idx) == 0) {
      fmpz_set_ui(poly->coeffs + idx, 1);
      bits_set++;
    }
  }
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniform(fmpz_poly_t poly, unsigned int coeffBitCnt) {
  const unsigned len = fmpz_poly_length(poly);

  for (unsigned int i = 0; i < len; i++)
    UniformRng::sample(poly->coeffs + i, coeffBitCnt);
}

/** @brief See header for description.
 */
void RandPolynom::sampleNormal(fmpz_poly_t poly, fmpz_t sigma, fmpz_t B) {
  const unsigned len = fmpz_poly_length(poly);

  for (unsigned int i = 0; i < len; i++)
    NormalRng::sample(poly->coeffs + i, sigma, B);
}
