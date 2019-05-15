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

/** @file encdec.hxx
 *  @brief EncDec - encryption/decryption class
 */

#ifndef __ENCDEC_HXX__
#define __ENCDEC_HXX__

#include "polyring.hxx"
#include "keys_all.hxx"

class EncDec {
public:
  /**
   * @brief Scales a polynomial ring element before encryption
   * @details Plaintext messages are polynomial ring elements, but before
   *    encryption each coefficient is multiplied by \c Delta parameter.
   *
   * @param pTxt polynomial ring object
   * @return scaled polynomial ring object
   */
  static void ScalePlainTextPoly(PolyRing& poly, const PolyRing& pTxt);

  /**
   * @brief Decrypts a ciphertext together with its noise (as polynomial)
   *
   * @param cTxt ciphertext object to decrypt
   * @param secretKey secret key
   * @param pNoise output noise polynomial
   *
   * @return the decrypted polynomial ring element and the noise polynomial
   */
  static PolyRing DecryptPolyAndNoise(const CipherText& cipherTxt, const PolyRing& secretKey, PolyRing& pNoise);

  /**
   * @brief Encrypts a polynomial ring element
   *
   * @param pTxt polynomial ring element to encrypt
   * @param publicKey public key
   *
   * @return a ciphertext object with encrypted polynomial
   */
  static CipherText EncryptPoly(const PolyRing& pTxt, const CipherText& publicKey);

  /**
   * @brief Builds a "plain" ciphertext object
   * @details Builds a "plain" ciphertext object used in combined computations
   *    between plaintexts and ciphertexts
   *
   * @param pTxt polyring element to build into
   * @return a ciphertext object which represent the plaintext in
   *    ciphertext domain
   */
  static CipherText EncryptPoly(const PolyRing& pTxt);

  /**
   * @brief Decrypts a ciphertext
   *
   * @param cTxt ciphertext object to decrypt
   * @param secretKey secret key
   *
   * @return the decrypted polynomial ring element
   */
  static PolyRing DecryptPoly(const CipherText& cTxt, const PolyRing& secretKey);

  /**
   * @brief Encrypts a single plaintext
   * @details Encrypts a plaintext element into the zero-degree coefficient
   *    of ciphertext polynomial
   *
   * @param int plaintext to encrypt (integer modulo T)
   * @param publicKey encryption key
   *
   * @return a ciphertext object which encrypts the plaintext
   */
  static CipherText Encrypt(const unsigned int pTxt, const CipherText& publicKey);

  /**
   * @brief Builds a "plain" ciphertext object
   * @details Builds a "plain" ciphertext object used in combined computations
   *    between plaintexts and ciphertexts
   *
   * @param int plaintext to build into
   * @return a ciphertext object which represent the plaintext in
   *    ciphertext domain
   */
  static CipherText Encrypt(const unsigned int pTxt);

  /**
   * @brief Decrypts a ciphertext
   * @details Decrypts a ciphertext and returns the zero-degree coefficient
   *
   * @param cTxt ciphertext object to decrypt
   * @param secretKey decryption key
   *
   * @return a plaintext element (integer modulo T)
   */
  static unsigned int Decrypt(const CipherText& cTxt, const PolyRing& secretKey);

  /**
   * @brief Compute the noise amplitude in a ciphertext
   *
   * @param cTxt ciphertext
   * @param secretKey decryption key
   *
   * @return number of bits of noise in the ciphertext
   */
  static unsigned int Noise(const CipherText& cTxt, const PolyRing& secretKey);

  /**
   * @brief Compute the noise amplitude in a ciphertext (float version)
   *
   * @param cTxt ciphertext
   * @param secretKey decryption key
   *
   * @return number of bits of noise in the ciphertext
   */
  static double NoiseDbl(const CipherText& cTxt, const PolyRing& secretKey);

  /**
   * @brief Compute the noise amplitude of a noise polynomial.
   *    Use \c EncDec::DecryptPolyAndNoise to find the noise polynomial
   *    The polynomial is altered.
   *
   * @param polyNoise noise polynomial
   *
   * @return number of bits of noise in the ciphertext (floating-point)
   */
  static double NoiseDbl(const PolyRing& polyNoise);

private:
  static thread_local PolyRing tmp_poly;
  static thread_local CipherText tmp_ctxt;
};

#endif
