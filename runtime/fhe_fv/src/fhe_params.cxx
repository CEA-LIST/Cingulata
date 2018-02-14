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

#include "fhe_params.hxx"

#include <assert.h>
#include <iostream>
#include <flint/arith.h>
#include <pugixml.hpp>

using namespace pugi;
using namespace std;

/** @brief See header for description
 */
unsigned int FheParams::T;

/** @brief See header for description
 */
fmpz_t FheParams::Q;

/** @brief See header for description
 */
unsigned int FheParams::Q_bitsize;

/** @brief See header for description
 */
fmpz_t FheParams::Delta;

/** @brief See header for description
 */
fmpz_t FheParams::P;

/** @brief See header for description
 */
fmpz_t FheParams::PQ;

/** @brief See header for description
 */
bool FheParams::IsPowerOfTwoCyclotomic;

/** @brief See header for description
 */
fmpz_poly_t FheParams::PolyRingModulo;

/** @brief See header for description
 */
fmpz_poly_powers_precomp_t FheParams::PolyRingModuloInv;

/** @brief See header for description
 */
unsigned int FheParams::D;

/** @brief See header for description
 */
fmpz_t FheParams::SIGMA;

/** @brief See header for description
 */
fmpz_t FheParams::B;

/** @brief See header for description
 */
fmpz_t FheParams::SIGMA_K;

/** @brief See header for description
 */
fmpz_t FheParams::B_K;

/** @brief See header for description
 */
unsigned int FheParams::SK_H;

/** @brief See header for description
 */
unsigned int FheParams::POLY_RW_BASE;

/** @brief See header for description
 */
FheParams::_init::_init() {
  FheParams::T = 0;
  FheParams::D = 0;
  FheParams::SK_H = 0;
  FheParams::POLY_RW_BASE = 62; //@todo read it from xml file
  
  fmpz_init(FheParams::SIGMA);
  fmpz_init(FheParams::B);
  fmpz_init(FheParams::SIGMA_K);
  fmpz_init(FheParams::B_K);
  fmpz_init(FheParams::Q);
  fmpz_init(FheParams::P);
  fmpz_init(FheParams::PQ);
  fmpz_init(FheParams::Delta);

  fmpz_poly_init(FheParams::PolyRingModulo);
}

/** @brief See header for description
 */
FheParams::_init::~_init() {
  fmpz_clear(FheParams::SIGMA);
  fmpz_clear(FheParams::B);
  fmpz_clear(FheParams::SIGMA_K);
  fmpz_clear(FheParams::B_K);
  fmpz_clear(FheParams::Q);
  fmpz_clear(FheParams::P);
  fmpz_clear(FheParams::PQ);
  fmpz_clear(FheParams::Delta);
  
  fmpz_poly_clear(FheParams::PolyRingModulo);
  fmpz_poly_powers_clear(FheParams::PolyRingModuloInv);

  flint_cleanup();
}

/** @brief See header for description
 */
FheParams::_init FheParams::_initializer;

/** @brief Helper functions for XML parsing
 */
void parseParamsPolyCoeffs(xml_node node, fmpz_poly_t poly) {
  for (xml_node n = node.first_child(); n; n = n.next_sibling()) {
    unsigned int value = n.child("value").text().as_uint();
    unsigned int degree = n.child("degree").text().as_uint();

    fmpz_poly_set_coeff_ui(poly, degree, value);
  }
}

/** @brief Helper functions for XML parsing
 */
void parseParamsPr(xml_node node) {
  xml_node node1;

  node1 = node.child("cyclotomic_polynomial");
  if (not node1.empty() and not node1.child("coeffs").empty()) {
    parseParamsPolyCoeffs(node1.child("coeffs"), FheParams::PolyRingModulo);
  } else if (not node1.child("index").empty()) {
    unsigned int index = node1.child("index").text().as_uint();
    arith_cyclotomic_polynomial(FheParams::PolyRingModulo, index);
  } else {
    cerr << "Error parsing XML params file: " <<
      "no ring modulo polynomial specified" << endl;
    exit(0);
  }
}

/** @brief Helper functions for XML parsing
 */
void parseParamsPt(xml_node node) {
  FheParams::T = node.child("coeff_modulo").text().as_uint();
}

/** @brief Helper functions for XML parsing
 */
void parseParamsCt(xml_node node) {
  int r;

  if (node.child("coeff_modulo_log2")) {
    unsigned int log2_q = node.child("coeff_modulo_log2").text().as_uint();
    fmpz_set_ui(FheParams::Q, 2);
    fmpz_pow_ui(FheParams::Q, FheParams::Q, log2_q);  
    FheParams::Q_bitsize = log2_q;
  } else {
    r = fmpz_set_str(FheParams::Q, node.child_value("coeff_modulo"), 10);
    assert(r == 0);
    FheParams::Q_bitsize = fmpz_clog_ui(FheParams::Q, 2); 
  }

  node = node.child("normal_distribution");
  
  r = fmpz_set_str(FheParams::SIGMA, node.child_value("sigma"), 10);
  assert(r == 0);

  r = fmpz_set_str(FheParams::B, node.child_value("bound"), 10);
  assert(r == 0);
}

/** @brief Helper functions for XML parsing
 */
void parseParamsLi(xml_node node) {
  int r;

  if (node.child("coeff_modulo_log2")) {
    unsigned int log2_p = node.child("coeff_modulo_log2").text().as_uint();
    fmpz_set_ui(FheParams::P, 2);
    fmpz_pow_ui(FheParams::P, FheParams::P, log2_p);  
  } else {
    r = fmpz_set_str(FheParams::P, node.child_value("coeff_modulo"), 10);
    assert(r == 0);
  }

  node = node.child("normal_distribution");

  r = fmpz_set_str(FheParams::SIGMA_K, node.child_value("sigma"), 10);
  assert(r == 0);

  r = fmpz_set_str(FheParams::B_K, node.child_value("bound"), 10);
  assert(r == 0);
}

/** @brief Helper functions for XML parsing
 */
void parseParamsSk(xml_node node) {
  FheParams::SK_H = node.child("hamming_weight").text().as_uint();
}

/** @brief See header for description
 */
void FheParams::readXml(const char* const fileName) {
  xml_document doc;

  xml_parse_result result = doc.load_file(fileName);

  if (result.status != status_ok) {
    cerr << "Cannot read parameters file '" << fileName
        << "' in method FheParams::readXml" << endl;
    exit(0);
  }

  xml_node params = doc.child("fhe_params");

  if (!params) {
    cerr << "Error parsing file '" << fileName
        << "' in method FheParams::readXml" << endl;
    exit(0);
  }

  parseParamsPr(params.child("polynomial_ring"));
  parseParamsPt(params.child("plaintext"));
  parseParamsCt(params.child("ciphertext"));
  parseParamsLi(params.child("linearization"));
  parseParamsSk(params.child("secret_key"));

  FheParams::computeParams();
}
  
/** @brief See header for description
 */
void FheParams::computeParams() {
  fmpz_mul(FheParams::PQ, FheParams::P, FheParams::Q);

  fmpz_fdiv_q_ui(FheParams::Delta, FheParams::Q, FheParams::T);

  /* Cyclotomic polynomial degree */
  FheParams::D = fmpz_poly_degree(FheParams::PolyRingModulo);

  /* Verify if it's a power of two cyclotomic polynomial */
  FheParams::IsPowerOfTwoCyclotomic = 
    FheParams::isPowerOfTwoCyclotomicPolynomial(FheParams::PolyRingModulo);

  /* Precompute the inverse of the cyclotomic polynomial
   *  used in ring modulo reduction if not power of two cyclotomic */
  if (not FheParams::IsPowerOfTwoCyclotomic) {
    fmpz_poly_powers_precompute(FheParams::PolyRingModuloInv,
                                FheParams::PolyRingModulo);
  }
}

/** @brief See header for description
 */
bool FheParams::isPowerOfTwoCyclotomicPolynomial(fmpz_poly_t poly) {
  int degree = fmpz_poly_degree(poly);

  /* Polynom degree must be a power of two*/
  if (degree != (1 << FLINT_CLOG2(degree))) {
    return false;
  }

  /* Only first and last polynomial coefficients are 1 */
  bool b = true;
  b &= (fmpz_poly_get_coeff_ui(poly, 0) == 1);
  b &= (fmpz_poly_get_coeff_ui(poly, degree) == 1);

  /* Other coefficients must be 0 */
  for (int i = 1; i < degree; i++) {
    b &= (fmpz_poly_get_coeff_ui(poly, i) == 0);
  }

  return b;
}

