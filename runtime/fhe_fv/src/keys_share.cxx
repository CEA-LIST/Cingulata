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


#include <iostream>
#include <string>

#include "keys_share.hxx"

using namespace std;


/** @brief See header for a description
 */
KeysShare::~KeysShare() {
  delete PublicKey;
  delete EvalKey;
}

/** @brief See header for a description
 */
void KeysShare::readPublicKey(FILE* const stream, const bool binary) {
  if (PublicKey != NULL) {
    delete PublicKey;
  }
  
  PublicKey = new CipherText();
  PublicKey->read(stream, binary);
}

/** @brief See header for a description
 */
void KeysShare::readPublicKey(const string& fileName, const bool binary) {
  FILE* stream;

  stream = fopen(fileName.c_str(), binary ? "rb" : "r");

  if (stream == NULL) {
    cout << "Cannot open public key file: '" << fileName << "'" << endl;
    exit(-1);
  }

  readPublicKey(stream, binary);
  fclose(stream);
}

/** @brief See header for a description
 */
void KeysShare::readEvalKey(FILE* const stream, const bool binary) {
  if (EvalKey != NULL) {
    delete PublicKey;
  }
  
  EvalKey = new CipherText();
  EvalKey->read(stream);
}

/** @brief See header for a description
 */
void KeysShare::readEvalKey(const string& fileName, const bool binary) {
  FILE* stream;

  stream = fopen(fileName.c_str(), binary ? "rb" : "r");

  if (stream == NULL) {
    cout << "Cannot open evaluation key file: '" << fileName << "'" << endl;
    exit(-1);
  }

  readEvalKey(stream, binary);
  fclose(stream);
}

/** @brief See header for a description
 */
void KeysShare::readKeys(const string& fileNamePrefix, const bool binary) {
  readPublicKey(fileNamePrefix + ".pk", binary);
  readEvalKey(fileNamePrefix + ".evk", binary);
}

/** @brief See header for a description
 */
void KeysShare::writePublicKey(FILE* const stream, const bool binary) {
  if (PublicKey != NULL) {
    PublicKey->write(stream, binary);
  }
}

/** @brief See header for a description
 */
void KeysShare::writeEvalKey(FILE* const stream, const bool binary) {
  if (EvalKey != NULL) {
    EvalKey->write(stream, binary);
  }
}

/** @brief See header for a description
 */
void KeysShare::writeKeys(const string& fileNamePrefix, const bool binary) {
  FILE* stream;

  stream = fopen((fileNamePrefix + ".pk").c_str(), binary ? "wb" : "w");
  writePublicKey(stream, binary);
  fclose(stream);

  stream = fopen((fileNamePrefix + ".evk").c_str(), binary ? "wb" : "w");
  writeEvalKey(stream, binary);
  fclose(stream);
}

