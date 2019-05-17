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

#include <flint/fmpq.h>
#include <flint/fmpz_vec.h>

using namespace std;

thread_local fmpz_poly_t PolyRing::sm_tmp_poly;

PolyRing::_init::_init() {
  fmpz_poly_init2(PolyRing::sm_tmp_poly,
                  2 * FheParams::D - 1); // 2x size for multiplication
}

PolyRing::_init::~_init() { fmpz_poly_clear(PolyRing::sm_tmp_poly); }

/** @brief See header for a description
 */
void PolyRing::write_fmpz(FILE *const stream, fmpz_t d, const bool binary) {
  if (binary) {
    fmpz_out_raw(stream, d);
  } else {
    char *buff = fmpz_get_str(NULL, FheParams::POLY_RW_BASE, d);
    fprintf(stream, "%s\n", buff);
    free(buff);
  }
}

/** @brief See header for a description
 */
void PolyRing::read_fmpz(fmpz_t d, FILE *const stream, const bool binary) {
  if (binary) {
    fmpz_inp_raw(d, stream);
  } else {
    char *buff;
    fscanf(stream, "%ms", &buff);
    fmpz_set_str(d, buff, FheParams::POLY_RW_BASE);
    free(buff);
  }
}

namespace {
void fmpz_copy(fmpz_poly_t out, const fmpz_poly_t inp) {
  assert(out->length >= inp->length);
  for (unsigned i = 0; i < inp->length; i++)
    fmpz_set(out->coeffs + i, inp->coeffs + i);
  for (unsigned i = inp->length; i < out->length; i++)
    fmpz_zero(out->coeffs + i);
}
} // namespace

/** @brief See header for a description
 */
PolyRing::PolyRing() {
  fmpz_poly_init2(polyData, FheParams::D);
  _fmpz_poly_set_length(polyData, FheParams::D);
}

/** @brief See header for a description
 */
PolyRing::PolyRing(fmpz_poly_t poly) : PolyRing() { fmpz_copy(polyData, poly); }

/** @brief See header for a description
 */
PolyRing::PolyRing(const PolyRing &prElem) : PolyRing() {
  fmpz_copy(polyData, prElem.polyData);
}

/**
 * @brief See header for a description
 */
PolyRing::PolyRing(const vector<unsigned int> &poly_coeff) : PolyRing() {
  unsigned int n = poly_coeff.size();
  if (n > length())
    n = length();
  for (unsigned int i = 0; i < n; ++i) {
    fmpz_poly_set_coeff_ui(polyData, i, poly_coeff[i]);
  }
}

/** @brief See header for a description
 */
PolyRing::~PolyRing() { fmpz_poly_clear(polyData); }

/** @brief See header for a description
 */
void PolyRing::modulo(PolyRing &prElem, const unsigned int q) {
  fmpz_t q_mpz;
  fmpz_init_set_ui(q_mpz, q);

  modulo(prElem, q_mpz);

  fmpz_clear(q_mpz);
}

/** @brief See header for a description
 */
void PolyRing::modulo(PolyRing &prElem, const fmpz_t q) {
  _fmpz_vec_scalar_mod_fmpz(prElem.polyData->coeffs, prElem.polyData->coeffs,
                            prElem.polyData->length, q);
  // _fmpz_vec_scalar_smod_fmpz(prElem.polyData->coeffs,
  // prElem.polyData->coeffs, prElem.polyData->length, q);
}

/** @brief See header for a description
 */
void PolyRing::negate(PolyRing &prElem) {
  fmpz_poly_neg(prElem.polyData, prElem.polyData);
}

/** @brief See header for a description
 */
void PolyRing::add(PolyRing &left, const PolyRing &right) {
  assert(left.polyData->length == right.polyData->length);
  _fmpz_poly_add(left.polyData->coeffs, left.polyData->coeffs,
                 left.polyData->length, right.polyData->coeffs,
                 right.polyData->length);
}

/** @brief See header for a description
 */
void PolyRing::sub(PolyRing &left, const PolyRing &right) {
  assert(left.polyData->length == right.polyData->length);
  _fmpz_poly_sub(left.polyData->coeffs, left.polyData->coeffs,
                 left.polyData->length, right.polyData->coeffs,
                 right.polyData->length);
}

/** @brief See header for a description
 */
void PolyRing::reduce(fmpz_poly_t out, const fmpz_poly_t inp) {
  if (FheParams::IsPowerOfTwoCyclotomic) {
    assert(out->length == FheParams::D);
    assert(inp->length >= FheParams::D);
    for (unsigned i = 0; i < inp->length - FheParams::D; i++)
      fmpz_sub(out->coeffs + i, inp->coeffs + i,
               inp->coeffs + i + FheParams::D);
    for (unsigned i = inp->length - FheParams::D; i < FheParams::D; i++)
      fmpz_set(out->coeffs + i, inp->coeffs + i);
  } else {
    assert(false);
    // fmpz_poly_rem_powers_precomp(out, inp, FheParams::PolyRingModulo,
    //                              FheParams::PolyRingModuloInv);
  }
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing &result, const PolyRing &left,
                        const PolyRing &right) {
  /* Multiply polynomials  */
  fmpz_poly_mul(PolyRing::sm_tmp_poly, left.polyData, right.polyData);

  /* Reduce the product by polynomial ring modulo */
  reduce(result.polyData, PolyRing::sm_tmp_poly);
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing &result, const PolyRing &right) {
  PolyRing::multiply(result, result, right);
}

/** @brief See header for a description
 */
void PolyRing::multiply(PolyRing &prElem, const fmpz_t t) {
  _fmpz_vec_scalar_mul_fmpz(prElem.polyData->coeffs, prElem.polyData->coeffs,
                            prElem.polyData->length, t);
}

/** @brief See header for a description
 */
void PolyRing::multiply_round(PolyRing &prElem, const unsigned int t,
                              const fmpz_t q) {
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

  /* Multiply each polynomial coefficient by \c frac, add 1/2 and set polynomial
   * coefficient*/
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
void PolyRing::square(PolyRing &prElem) {
  /* Square polynomial */
  fmpz_poly_sqr(PolyRing::sm_tmp_poly, prElem.polyData);

  /* Reduce the square by polynomial ring modulo */
  reduce(prElem.polyData, PolyRing::sm_tmp_poly);
}

void PolyRing::copy(PolyRing &out, const PolyRing &inp) {
  assert(out.length() == inp.length());
  fmpz_copy(out.polyData, inp.polyData);
}

/** @brief See header for a description
 */
PolyRing &PolyRing::operator=(const PolyRing &prElem) {
  if (this != &prElem) {
    copy(*this, prElem);
  }
  return *this;
}

/** @brief See header for a description
 */
void PolyRing::read(FILE *const stream, const bool binary) {
  fmpz_t size_fmpz;
  fmpz_init(size_fmpz);

  PolyRing::read_fmpz(size_fmpz, stream, binary);

  long size = fmpz_get_si(size_fmpz);

  assert(size <= FheParams::D);
  _fmpz_poly_set_length(polyData, size);

  for (unsigned int i = 0; i < length(); i++) {
    PolyRing::read_fmpz(getCoeff(i), stream, binary);
  }

  fmpz_clear(size_fmpz);
}

/** @brief See header for a description
 */
void PolyRing::write(FILE *const stream, const bool binary) const {
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
void PolyRing::print(FILE *const stream) const {
  int r = fmpz_poly_fprint_pretty(stream, polyData, "X");
  assert(r != 0);
}

/** @brief See header for a description
 */
unsigned int PolyRing::length() const {
  assert(fmpz_poly_length(polyData) == FheParams::D);
  return FheParams::D;
}
