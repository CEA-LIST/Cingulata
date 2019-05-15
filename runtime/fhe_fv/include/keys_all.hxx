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

/** @file keys_all.hxx
 *  @brief Manage all keys: secret, public and evaluation ones
 */

#ifndef __KEYS_ALL_HXX__
#define __KEYS_ALL_HXX__

#include <string>

#include "keys_share.hxx"
#include "polyring.hxx"

class KeysAll : public KeysShare {
  public:
    /** @brief Secret key
     */
    PolyRing* SecretKey;

    /** @brief Basic constructor
     */
    inline KeysAll(): KeysShare(), SecretKey(NULL) {}

    /** @brief Deallocates keys
     */
    ~KeysAll();

    /** @brief Read secret key from an input stream
     *
     *  @param in_io input stream from which read the key
     */
    void readSecretKey(FILE* const stream, const bool binary = true);

    /** @brief Read secret key from a file
     *
     *  @param fileName input file name from which read the key
     */
    void readSecretKey(const std::string& fileName, const bool binary = true);

    /** @brief Write secret key from an input stream
     *
     *  @param out_io output stream to which write the key
     */
    void writeSecretKey(FILE* const stream, const bool binary = true);

    /** @brief Read all keys from files with a given prefix
     *
     *  @param fileNamePrefix prefix of files names containing keys
     */
    void readKeys(const std::string& fileNamePrefix, const bool binary = true);

    /** @brief Write all keys to files with a given prefix
     *
     *  @param fileNamePrefix prefix of files names containing keys
     *  @param binary either to write keys as binary or as string
     */
    void writeKeys(const std::string& fileNamePrefix, const bool binary = true);
};

#endif
