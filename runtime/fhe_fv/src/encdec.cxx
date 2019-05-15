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

#include "encdec.hxx"
#include "rand_polynom.hxx"

#include "flint/fmpz_poly.h"
#include "flint/fmpz_mod_poly.h"

thread_local PolyRing EncDec::tmp_poly;
thread_local CipherText EncDec::tmp_ctxt(2);

/** @brief See header for description
 */
CipherText EncDec::EncryptPoly(const PolyRing& plainTxt_p, const CipherText& publicKey) {
  CipherText ctxt;
  EncryptPoly(ctxt, plainTxt_p, publicKey);
  return ctxt;
}

void EncDec::EncryptPoly(CipherText& ct, const PolyRing& plainTxt_p, const CipherText& publicKey)
{
  ct = publicKey;

  /* Sample uniform binary and normal distributed polynomials  */
  PolyRing &u = tmp_poly;
  RandPolynom::sampleUniformBinary(u.poly());

  CipherText &noise = tmp_ctxt;
  RandPolynom::sampleNormal(noise[0].poly(), FheParams::SIGMA, FheParams::B);
  RandPolynom::sampleNormal(noise[1].poly(), FheParams::SIGMA, FheParams::B);

  CipherText::multiply_comp(ct, {u, u});
  CipherText::add(ct, noise);

  /* Add to first cipher-text polynom the plaintext message */
  ScalePlainTextPoly(tmp_poly, plainTxt_p);
  PolyRing::add(ct[0], tmp_poly);

  CipherText::modulo(ct, FheParams::Q);
}

/** @brief See header for description
 */
CipherText EncDec::EncryptPoly(const PolyRing& plainTxt)
{
  CipherText ctxt(1);
  EncryptPoly(ctxt, plainTxt);
  return ctxt;
}

/** @brief See header for description
 */
void EncDec::EncryptPoly(CipherText& ctxt, const PolyRing& plainTxt)
{
  ScalePlainTextPoly(ctxt[0], plainTxt);
  ctxt.resize(1);
}

/** @brief See header for description
 */
PolyRing EncDec::DecryptPolyAndNoise(const CipherText& cTxt, const PolyRing& secretKey, PolyRing& pNoise)
{
  PolyRing &sk = tmp_poly;
  PolyRing::copy(sk, secretKey);

  pNoise = PolyRing(cTxt[0]);
  for (unsigned int i = 1; i < cTxt.size(); ++i) {
    PolyRing tmp(cTxt[i]);
    PolyRing::multiply(tmp, sk);
    PolyRing::add(pNoise, tmp);
    PolyRing::modulo(pNoise, FheParams::Q);

    if (i < cTxt.size()-1) {
      PolyRing::multiply(sk, secretKey);
    }
  }

  PolyRing pMsg(pNoise);
  PolyRing::multiply_round(pMsg, FheParams::T, FheParams::Q);
  PolyRing::modulo(pMsg, FheParams::T);

  PolyRing tmp(pMsg);
  PolyRing::multiply(tmp, FheParams::Delta);
  PolyRing::sub(pNoise, tmp);

  return pMsg;
}

/** @brief See header for description
 */
PolyRing EncDec::DecryptPoly(const CipherText& cipherTxt, const PolyRing& secretKey)
{
  PolyRing noisePoly;
  return EncDec::DecryptPolyAndNoise(cipherTxt, secretKey, noisePoly);
}

/** @brief See header for description
 */
void EncDec::ScalePlainTextPoly(PolyRing& poly, const PolyRing& plainTxt)
{
  PolyRing::copy(poly, plainTxt);
  PolyRing::modulo(poly, FheParams::T);
  PolyRing::multiply(poly, FheParams::Delta);
}

/** @brief See header for description
 */
CipherText EncDec::Encrypt(const unsigned int pTxt, const CipherText& publicKey)
{
  CipherText ctxt;
  Encrypt(ctxt, pTxt, publicKey);
  return ctxt;
}

void EncDec::Encrypt(CipherText& ctxt, const unsigned int pTxt, const CipherText& publicKey)
{
  PolyRing poly;
  poly.setCoeffUi(0, pTxt);
  EncryptPoly(ctxt, poly, publicKey);
}

/** @brief See header for description
 */
CipherText EncDec::Encrypt(const unsigned int pTxt)
{
  CipherText ctxt;
  Encrypt(ctxt, pTxt);
  return ctxt;
}

void EncDec::Encrypt(CipherText& ctxt, const unsigned int pTxt)
{
  PolyRing poly;
  poly.setCoeffUi(0, pTxt);
  EncryptPoly(ctxt, poly);
}


/** @brief See header for description
 */
unsigned int EncDec::Decrypt(const CipherText& cTxt, const PolyRing& secretKey)
{
  PolyRing poly = EncDec::DecryptPoly(cTxt, secretKey);
  return poly.getCoeffUi(0);
}

/** @brief See header for description
 */
unsigned int EncDec::Noise(const CipherText& cTxt, const PolyRing& secretKey)
{
  return (unsigned int)ceil(EncDec::NoiseDbl(cTxt, secretKey));
}

/** @brief See header for description
 */
double EncDec::NoiseDbl(const CipherText& cTxt, const PolyRing& secretKey)
{
  EncDec::DecryptPolyAndNoise(cTxt, secretKey, tmp_poly);
  return EncDec::NoiseDbl(tmp_poly);
}

/** @brief See header for description
 */
double EncDec::NoiseDbl(const PolyRing& pNoise)
{
  double noise = 0.0;
  for (unsigned int i = 0; i < pNoise.length(); ++i) {
    fmpz* coeff = pNoise.getCoeff(i);

    if (fmpz_cmp(coeff, FheParams::Delta) >= 0)
      fmpz_sub(coeff, coeff, FheParams::Q);

    fmpz_abs(coeff, coeff); //really need this?

    double noiseCoeff = fmpz_dlog(coeff) / log(2);
    noise = (noiseCoeff > noise) ? noiseCoeff : noise;
  }

  return noise;
}
