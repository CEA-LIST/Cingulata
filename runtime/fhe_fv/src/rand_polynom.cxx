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
#include "uniform.hxx"
#include "normal.hxx"

/** @brief See header for description.
 */
void RandPolynom::populateBinaryPolyRing(fmpz_poly_t poly, fmpz_t num) {
  fmpz_poly_bit_unpack_unsigned(poly, num, 1);
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniformBinary(fmpz_poly_t poly, unsigned int len) {
  fmpz_t num;
  fmpz_init(num);

  UniformRng::sample(num, len);

  populateBinaryPolyRing(poly, num);

  fmpz_clear(num);
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniformBinary(fmpz_poly_t poly, unsigned int len, unsigned int hammingWeight) {
  fmpz_t num;
  fmpz_init(num);

  UniformRng::sample(num, len, hammingWeight);

  populateBinaryPolyRing(poly, num);

  fmpz_clear(num);
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniform(fmpz_poly_t poly, unsigned int len, unsigned int coeffBitCnt) {
  fmpz_t d;
  fmpz_init(d);
  
  for (unsigned int i = 0; i < len; i++){
    UniformRng::sample(d, coeffBitCnt);
    fmpz_poly_set_coeff_fmpz(poly, i, d);
  }

  fmpz_clear(d);
}

/** @brief See header for description.
 */
void RandPolynom::sampleUniform(fmpz_poly_t poly, unsigned int len, fmpz_t q) {
  sampleUniform(poly, len, fmpz_sizeinbase(q, 2));
}

/** @brief See header for description.
 */
void RandPolynom::sampleNormal(fmpz_poly_t poly, unsigned int len, fmpz_t sigma, fmpz_t B) {
  fmpz_t d;
  fmpz_init(d);
  
  for (unsigned int i = 0; i < len; i++){
    NormalRng::sample(d, sigma, B);
    fmpz_poly_set_coeff_fmpz(poly, i, d);
  }

  fmpz_clear(d);
}


