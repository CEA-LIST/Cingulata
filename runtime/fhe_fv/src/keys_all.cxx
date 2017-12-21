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

#include "keys_all.hxx"

#include <iostream>

using namespace std;

/** @brief See header for a description
 */
KeysAll::~KeysAll() {
  delete SecretKey;
}

/** @brief See header for a description
 */
void KeysAll::readSecretKey(FILE* const stream, const bool binary) {
  if (SecretKey != NULL) {
    delete SecretKey;
  }
  
  SecretKey = new PolyRing();
  SecretKey->read(stream, binary);
}

/** @brief See header for a description
 */
void KeysAll::readSecretKey(const string& fileName, const bool binary) {
  FILE* stream;

  stream = fopen(fileName.c_str(), binary ? "rb" : "r");

  if (stream == NULL) {
    cerr << "Cannot open secret key file: '" << fileName << "'" << endl;
    exit(-1);
  }

  readSecretKey(stream, binary);
  fclose(stream);
}

/** @brief See header for a description
 */
void KeysAll::writeSecretKey(FILE* const stream, const bool binary) {
  if (SecretKey != NULL) {
    SecretKey->write(stream, binary);
  }
}

/** @brief See header for a description
 */
void KeysAll::readKeys(const string& fileNamePrefix, const bool binary) {
  readSecretKey(fileNamePrefix + ".sk", binary);

  KeysShare::readKeys(fileNamePrefix, binary);
}

/** @brief See header for a description
 */
void KeysAll::writeKeys(const string& fileNamePrefix, const bool binary) {
  FILE* stream;

  stream = fopen((fileNamePrefix + ".sk").c_str(), binary ? "wb" : "w");
  writeSecretKey(stream, binary);
  fclose(stream);

  KeysShare::writeKeys(fileNamePrefix, binary);
}


