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

#include "normal.hxx"
#include "uniform.hxx"

#include <assert.h>
#include <gmp.h>
#include <stdlib.h>

/** @brief See header for description.
 */
gmp_randstate_t NormalRng::randstate;

/** @brief See header for description.
 */
void NormalRng::init() {
  gmp_randinit_lc_2exp_size(randstate, 64);

  fmpz_t seed;
  fmpz_init(seed);
  UniformRng::sample(seed, 64);

  mpz_t seed1;
  mpz_init(seed1);
  fmpz_get_mpz(seed1, seed);

  gmp_randseed(randstate, seed1);

  fmpz_clear(seed);
  mpz_clear(seed1);
}

/** @brief See header for description.
 */
NormalRng::_init::_init() { NormalRng::init(); }

/** @brief See header for description.
 */
NormalRng::_init::~_init() {
  gmp_randclear(randstate);
  mpfr_free_cache();
}

/** @brief See header for description.
 */
NormalRng::_init NormalRng::_initializer;

/** @brief See header for description.
 */
void NormalRng::sample(mpfr_t mpfr_num, mpfr_t mpfr_sigma) {
  mpfr_grandom(mpfr_num, NULL, randstate, MPFR_RNDNA);

  mpfr_mul(mpfr_num, mpfr_num, mpfr_sigma, MPFR_RNDNA);
}

/** @brief See header for description.
 */
void NormalRng::sample(fmpz_t num_p, const fmpz_t sigma_p, const fmpz_t B) {
  mpz_t sigma, num;

  mpz_init(sigma);
  fmpz_get_mpz(sigma, sigma_p);

  mpz_init(num);

  int prec = fmpz_sizeinbase(B, 2) + 2;

  mpfr_t mpfr_num;
  mpfr_init2(mpfr_num, prec);

  mpfr_t mpfr_sigma;
  mpfr_init(mpfr_sigma);
  mpfr_set_z(mpfr_sigma, sigma, MPFR_RNDNA);

  NormalRng::sample(mpfr_num, mpfr_sigma);

  mpfr_get_z(num, mpfr_num, MPFR_RNDNA);

  mpfr_clear(mpfr_num);
  mpfr_clear(mpfr_sigma);

  fmpz_set_mpz(num_p, num);

  mpz_clear(sigma);
  mpz_clear(num);
}
