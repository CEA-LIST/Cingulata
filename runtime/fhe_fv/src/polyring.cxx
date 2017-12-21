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

#include "polyring.hxx"
#include "fhe_params.hxx"

#include <iostream>
#include <sstream>
#include <stdlib.h>

#include <flint/fmpz_vec.h>
#include <flint/fmpq.h>

using namespace std;

/** @brief See header for a description
 */
void PolyRing::write_fmpz(FILE* const stream, fmpz_t d, const bool binary) {
  if (binary) {
    fmpz_out_raw(stream, d);
  } else {
    char* buff = fmpz_get_str(NULL, FheParams::POLY_RW_BASE, d);
    fprintf(stream, "%s\n", buff);
    free(buff);
  }
}

/** @brief See header for a description
 */
void PolyRing::read_fmpz(fmpz_t d, FILE* const stream, const bool binary) {
  if (binary) {
    fmpz_inp_raw(d, stream);
  } else {
    char* buff;
    fscanf(stream, "%ms", &buff);
    fmpz_set_str(d, buff, FheParams::POLY_RW_BASE);
    free(buff);
  }
}

/** @brief See header for a description
 */
PolyRing::PolyRing() {
  fmpz_poly_init2(this->polyData, FheParams::D);
}

/** @brief See header for a description
 */
PolyRing::PolyRing(fmpz_poly_t poly, bool copy) {
  if (copy) {
    fmpz_poly_init2(this->polyData, FheParams::D);
    fmpz_poly_set(this->polyData, poly);
  } else {
    *(this->polyData) = *poly;
  }
  reduce(*this);
}

/** @brief See header for a description
 */
PolyRing::PolyRing(const PolyRing& prElem) {
  fmpz_poly_init2(this->polyData, FheParams::D);
  fmpz_poly_set(this->polyData, prElem.polyData);
}

/**
 * @brief See header for a description
 */
PolyRing::PolyRing(const vector<unsigned int>& poly_coeff) {
  fmpz_poly_init2(this->polyData, FheParams::D);
  unsigned int n = poly_coeff.size();
  if (n > FheParams::D) n = FheParams::D;
  for (unsigned int i = 0; i < n; ++i) {
    fmpz_poly_set_coeff_ui(this->polyData, i, poly_coeff[i]);
  }
}

/** @brief See header for a description
 */
PolyRing::~PolyRing() {
  fmpz_poly_clear(this->polyData);
}

/** @brief See header for a description
 */
void PolyRing::modulo(PolyRing& prElem, const unsigned int q) {
  fmpz_t q_mpz;
  fmpz_init_set_ui(q_mpz, q);

  modulo(prElem, q_mpz);

  fmpz_clear(q_mpz);
}

/** @brief See header for a description
 */
void PolyRing::modulo(PolyRing& prElem, const fmpz_t q) {
  fmpz_poly_scalar_mod_fmpz(prElem.polyData, prElem.polyData, q);  // [O; q)
  //fmpz_poly_scalar_smod_fmpz(prElem.polyData, prElem.polyData, q);  // (-q/2;q/2]
}

/** @brief See header for a description
 */
void PolyRing::negate(PolyRing& prElem) {
  fmpz_poly_neg(prElem.polyData, prElem.polyData);
}

/** @brief See header for a description
 */
void PolyRing::add(PolyRing& left, const PolyRing& right) {
  fmpz_poly_add(left.polyData, left.polyData, right.polyData);
}

/** @brief See header for a description
 */
void PolyRing::sub(PolyRing& left, const PolyRing& right) {
  fmpz_poly_sub(left.polyData, left.polyData, right.polyData);
}

/** @brief See header for a description
 */
void PolyRing::reduce(PolyRing& prElem) {
  if (FheParams::IsPowerOfTwoCyclotomic) {
    /* Use simplified modulo operation for
     *  power of two cyclotomic polynomials */
    if (prElem.length() > FheParams::D) {
      _fmpz_vec_sub(prElem.polyData->coeffs,
                    prElem.polyData->coeffs,
                    prElem.polyData->coeffs + FheParams::D,
                    prElem.length() - FheParams::D);
      fmpz_poly_truncate(prElem.polyData, FheParams::D);
    }
  } else {
    fmpz_poly_rem_powers_precomp(prElem.polyData, prElem.polyData,
        FheParams::PolyRingModulo, FheParams::PolyRingModuloInv);
  }
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing& result, const PolyRing& left,
                              const PolyRing& right) {

  /* Multiply polynomials  */
  fmpz_poly_mul(result.polyData, left.polyData, right.polyData);
  //fmpz_poly_mul_karatsuba(result.polyData, left.polyData, right.polyData);

  /* Reduce the product by polynomial ring modulo */
  reduce(result);
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing& result, const PolyRing& right) {
  PolyRing prod;
  PolyRing::multiply(prod, result, right);
  result = prod;
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing& prElem, const fmpz_t t) {
  fmpz_poly_scalar_mul_fmpz(prElem.polyData, prElem.polyData, t);
}

/** @brief See header for a description
 */
void PolyRing::multiply_round(PolyRing& prElem, const  unsigned int t, const fmpz_t q) {
  fmpq_t b, frac, r;
  fmpq_init(b);

  /* Initialize rational number \c t/q */
  fmpq_init(frac);
  fmpz_set_ui(fmpq_numref(frac), t);
  fmpz_set(fmpq_denref(frac), q);
  fmpq_canonicalise(frac);

  /* Rounding to nearest integer doesn't exist in gmp,
   * so do it by adding 1/2 and flooring */
  fmpq_init(r);
  fmpz_set_ui(fmpq_numref(r), 1);
  fmpz_set_ui(fmpq_denref(r), 2);

  /* Multiply each polynomial coefficient by \c frac, add 1/2 and set polynomial coefficient*/
  for (unsigned int i = 0; i < prElem.length(); i++) {
    fmpq_set(b, frac);
    fmpq_mul_fmpz(b, b, prElem.getCoeff(i));
    fmpq_add(b, b, r);
    fmpz_fdiv_q(prElem.getCoeff(i), fmpq_numref(b), fmpq_denref(b));
  }

  fmpq_clear(b);
  fmpq_clear(frac);
  fmpq_clear(r);
}

/** @brief See header for a description
 */
void PolyRing::square(PolyRing& prElem) {
  /* Square polynomial */
  fmpz_poly_sqr(prElem.polyData, prElem.polyData);

  /* Reduce the square by polynomial ring modulo */
  reduce(prElem);
}

/** @brief See header for a description
 */
PolyRing& PolyRing::operator=(const PolyRing& prElem) {
  if (this != &prElem) {
    fmpz_poly_set(this->polyData, prElem.polyData);
  }
  return *this;
}

/** @brief See header for a description
 */
void PolyRing::read(FILE* const stream, const bool binary) {
  fmpz_t size_fmpz;
  fmpz_init(size_fmpz);

  PolyRing::read_fmpz(size_fmpz, stream, binary);

  long size = fmpz_get_si(size_fmpz);

  fmpz_poly_realloc(polyData, size);
  _fmpz_poly_set_length(polyData, size);

  for (unsigned int i = 0; i < length(); i++) {
    PolyRing::read_fmpz(getCoeff(i), stream, binary);
  }

  fmpz_clear(size_fmpz);
}

/** @brief See header for a description
 */
void PolyRing::write(FILE* const stream, const bool binary) const {
  fmpz_t d;
  fmpz_init_set_ui(d, length());

  PolyRing::write_fmpz(stream, d, binary);

  for (unsigned int i = 0; i < length(); i++) {
    PolyRing::write_fmpz(stream, getCoeff(i), binary);
  }

  fmpz_clear(d);
}

/** @brief See header for a description
 */
void PolyRing::print(FILE* const stream) const {
  int r = fmpz_poly_fprint_pretty(stream, this->polyData, "X");
  assert(r != 0);
}

/** @brief See header for a description
 */
unsigned int PolyRing::length() const
{
  return fmpz_poly_length(polyData);
}
