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

#include "public_key.hxx"
#include "rand_polynom.hxx"

#include <iostream>

using namespace std;

void PublicKey::read(const std::string &fileName, const bool binary) {
  FILE *stream = fopen(fileName.c_str(), binary ? "rb" : "r");

  if (stream == nullptr) {
    cout << "PublicKey::read -- Cannot open public key file: '" << fileName
         << "'" << endl;
    abort();
  }

  m_pk.read(stream, binary);
  m_evk.read(stream, binary);

  fclose(stream);
}

void PublicKey::write(const std::string &fileName, const bool binary) const {
  FILE *stream = fopen(fileName.c_str(), binary ? "wb" : "w");

  if (stream == nullptr) {
    cout << "PublicKey::write -- Cannot open public key file: '" << fileName
         << "'" << endl;
    abort();
  }

  m_pk.write(stream, binary);
  m_evk.write(stream, binary);

  fclose(stream);
}

void PublicKey::generate(const SecretKey &p_sk) {
  generate_pk(p_sk);
  generate_evk_v2(p_sk);
}

void PublicKey::generate_pk(const SecretKey &p_sk) {
  PolyRing &b = m_pk[0];
  PolyRing &a = m_pk[1];

  /* Sample a <- Rq and e <- \chi */
  RandPolynom::sampleUniform(a.poly(), FheParams::Q_bitsize);

  PolyRing e;
  RandPolynom::sampleNormal(e.poly(), FheParams::SIGMA, FheParams::B);

  /* Compute b = -(a . sk + e) mod q */
  PolyRing::copy(b, a);
  PolyRing::multiply(b, p_sk.get());
  PolyRing::add(b, e);
  PolyRing::negate(b);
  PolyRing::modulo(b, FheParams::Q);
}

void PublicKey::generate_evk_v2(const SecretKey &p_sk) {
  /* Re-linearization version 2 evaluation key */
  PolyRing &b = m_evk[0];
  PolyRing &a = m_evk[1];

  /* Sample a <- Rpq and e <- \chi */
  RandPolynom::sampleUniform(a.poly(), FheParams::PQ_bitsize);

  PolyRing e;
  RandPolynom::sampleNormal(e.poly(), FheParams::SIGMA_K, FheParams::B_K);

  /* Compute b = -(a . sk + e) */
  PolyRing::copy(b, a);
  PolyRing::multiply(b, p_sk.get());
  PolyRing::add(b, e);
  PolyRing::negate(b);

  /* Compute b += p . sk^2 mod p.q */
  PolyRing sk_copy(p_sk.get());
  PolyRing::square(sk_copy);
  PolyRing::multiply(sk_copy, FheParams::P);
  PolyRing::add(b, sk_copy);
  PolyRing::modulo(b, FheParams::PQ);
}
