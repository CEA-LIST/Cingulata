/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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

#include "ciphertext.hxx"
#include "fhe_params.hxx"
#include "polyring.hxx"
#include "rand_polynom.hxx"

using namespace std;

void KeyGen::generateSecretKey() {
  keysAll.SecretKey = new PolyRing();
  PolyRing &sk = (*keysAll.SecretKey);
  RandPolynom::sampleUniformBinary(sk.poly(), FheParams::SK_H);
}

void KeyGen::generatePublicKey() {
  keysAll.PublicKey = new CipherText(2);
  PolyRing &b = (*keysAll.PublicKey)[0];
  PolyRing &a = (*keysAll.PublicKey)[1];

  /* Sample a <- Rq and e <- \chi */
  RandPolynom::sampleUniform(a.poly(), FheParams::Q_bitsize);

  PolyRing e;
  RandPolynom::sampleNormal(e.poly(), FheParams::SIGMA, FheParams::B);

  /* Compute b = -(a . sk + e) mod q */
  PolyRing::copy(b, a);
  PolyRing::multiply(b, *(keysAll.SecretKey));
  PolyRing::add(b, e);
  PolyRing::negate(b);
  PolyRing::modulo(b, FheParams::Q);
}

void KeyGen::generateEvalKey() {
  /* Re-linearization version 2 evaluation key */
  keysAll.EvalKey = new CipherText(2);
  PolyRing &b = (*keysAll.EvalKey)[0];
  PolyRing &a = (*keysAll.EvalKey)[1];

  /* Sample a <- Rpq and e <- \chi */
  RandPolynom::sampleUniform(a.poly(), FheParams::PQ_bitsize);

  PolyRing e;
  RandPolynom::sampleNormal(e.poly(), FheParams::SIGMA_K, FheParams::B_K);

  /* Compute b = -(a . sk + e) */
  PolyRing::copy(b, a);
  PolyRing::multiply(b, *(keysAll.SecretKey));
  PolyRing::add(b, e);
  PolyRing::negate(b);

  /* Compute b += p . sk^2 mod p.q */
  PolyRing sk_copy(*(keysAll.SecretKey));
  PolyRing::square(sk_copy);
  PolyRing::multiply(sk_copy, FheParams::P);
  PolyRing::add(b, sk_copy);
  PolyRing::modulo(b, FheParams::PQ);
}

void KeyGen::generateKeys() {
  generateSecretKey();
  generatePublicKey();
  generateEvalKey();
}

void KeyGen::writeKeys(const string& fileNamePrefix, const bool binary) {
  keysAll.writeKeys(fileNamePrefix, binary);
}
