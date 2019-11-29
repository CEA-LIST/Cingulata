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

#include "keygen.hxx"

#include "fhe_params.hxx"
#include "rand_polynom.hxx"
#include "polyring.hxx"
#include "ciphertext.hxx"

#include <iostream>
#include <fstream>


using namespace std;

void KeyGen::generateSecretKey() {
  fmpz_poly_t tmp;
  fmpz_poly_init(tmp);

  RandPolynom::sampleUniformBinary(tmp, FheParams::D, FheParams::SK_H);
  
  keysAll.SecretKey = new PolyRing(tmp);

  fmpz_poly_clear(tmp);
}

void KeyGen::generatePublicKey() {
  fmpz_poly_t tmp;
  fmpz_poly_init(tmp);
  
  /* Sample a <- Rq and e <- \chi */
  RandPolynom::sampleUniform(tmp, FheParams::D, FheParams::Q);
  PolyRing *a = new PolyRing(tmp);

  //RandPolynom::sampleNormal(tmp, FheParams::D, FheParams::SIGMA, FheParams::B);
  if (FheParams::SK_H == -1)
  {
        RandPolynom::sampleUniformBinary(tmp, FheParams::D);
  }
  else
  {
          RandPolynom::sampleUniformBinary(tmp, FheParams::D, FheParams::SK_H);
  }

  PolyRing e(tmp);

  /* Compute ct1 = -(a . sk + e) mod q */
  PolyRing *ct1 = new PolyRing(*a);
  PolyRing::multiply(*ct1, *(keysAll.SecretKey));
  PolyRing::add(*ct1, e);
  PolyRing::negate(*ct1);
  PolyRing::modulo(*ct1, FheParams::Q);

  /* Store key */
  keysAll.PublicKey = new CipherText(ct1, a);

  fmpz_poly_clear(tmp);
}

void KeyGen::generateEvalKey() {
  fmpz_poly_t tmp;
  fmpz_poly_init(tmp);

  /* Re-linearization version 1 evaluation key */

  /* Re-linearization version 2 evaluation key */  
  /* Sample a <- Rpq and e <- \chi */
  RandPolynom::sampleUniform(tmp, FheParams::D, FheParams::PQ);
  PolyRing *a = new PolyRing(tmp);
  
  RandPolynom::sampleNormal(tmp, FheParams::D, FheParams::SIGMA_K, FheParams::B_K);
  PolyRing e(tmp);

  /* Compute ct1 = -(a . sk + e) */
  PolyRing *ct1 = new PolyRing(*a);
  PolyRing::multiply(*ct1, *(keysAll.SecretKey));
  PolyRing::add(*ct1, e);
  PolyRing::negate(*ct1);

  /* Compute ct1 += p . sk^2 mod p.q */
  PolyRing sk_copy(*(keysAll.SecretKey));
  PolyRing::square(sk_copy);
  PolyRing::multiply(sk_copy, FheParams::P);
  PolyRing::add(*ct1, sk_copy);  
  PolyRing::modulo(*ct1, FheParams::PQ);
  
  /* Store key */
  keysAll.EvalKey = new CipherText(ct1, a);

  fmpz_poly_clear(tmp);
}

void KeyGen::generateKeys() {
  generateSecretKey();
  generatePublicKey();
  generateEvalKey();
}

void KeyGen::writeKeys(const string& fileNamePrefix, const bool binary) {
  keysAll.writeKeys(fileNamePrefix, binary);
}
