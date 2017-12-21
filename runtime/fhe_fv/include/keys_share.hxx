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

/** @file keys_share.hxx
 *  @brief Manage shared keys: public and evaluation ones
 */

#ifndef __KEYS_SHARE_HXX__
#define __KEYS_SHARE_HXX__

#include "ciphertext.hxx"

#include <string>

class CipherText;

class KeysShare {
  public:
    /** @brief Public key
     */
    CipherText* PublicKey;

    /** @brief Evaluation key
     */
    CipherText* EvalKey;

    /** @brief Basic constructor
     */
    inline KeysShare(): PublicKey(NULL), EvalKey(NULL) {}

    /** @brief Deallocates keys
     */
    ~KeysShare();

    /** @brief Read public key from an input stream
     *
     *  @param in_io input stream from which read the key
     */
    void readPublicKey(FILE* const stream, const bool binary = true);

    /** @brief Read public key from a file
     *
     *  @param fileName input file name from which read the key
     */
    void readPublicKey(const std::string& fileName, const bool binary = true);

    /** @brief Read evaluation key from an input stream
     *
     *  @param in_io input stream from which read the key
     */
    void readEvalKey(FILE* const stream, const bool binary = true);

    /** @brief Read evaluation key from a file
     *
     *  @param fileName input file name from which read the key
     */
    void readEvalKey(const std::string& fileName, const bool binary = true);

    /** @brief Read all keys from files with a given prefix
     *
     *  @param fileNamePrefix prefix of files names containing keys
     */
    void readKeys(const std::string& fileNamePrefix, const bool binary = true);
  
    /** @brief Write public key from an input stream
     *
     *  @param out_io output stream to which write the key
     */
    void writePublicKey(FILE* const stream, const bool binary = true);

    /** @brief Write evaluation key from an input stream
     *
     *  @param out_io output stream to which write the key
     */
    void writeEvalKey(FILE* const stream, const bool binary = true);

    /** @brief Write all keys to files with a given prefix
     *
     *  @param fileNamePrefix prefix of files names containing keys
     */
    void writeKeys(const std::string& fileNamePrefix, const bool binary = true);
};

#endif
