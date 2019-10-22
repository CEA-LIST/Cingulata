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

#include "rand_polynom.hxx"
#include "secret_key.hxx"

#include <iostream>

using namespace std;

void SecretKey::read(const std::string &fileName, const bool binary) {
  FILE *stream = fopen(fileName.c_str(), binary ? "rb" : "r");

  if (stream == nullptr) {
    cout << "SecretKey::read -- Cannot open public key file: '" << fileName
         << "'" << endl;
    abort();
  }

  m_sk.read(stream, binary);
  fclose(stream);
}

void SecretKey::write(const std::string &fileName, const bool binary) const {
  FILE *stream = fopen(fileName.c_str(), binary ? "wb" : "w");

  if (stream == nullptr) {
    cout << "SecretKey::write -- Cannot open public key file: '" << fileName
         << "'" << endl;
    abort();
  }

  m_sk.write(stream, binary);
  fclose(stream);
}

void SecretKey::generate() {
  RandPolynom::sampleUniformBinary(m_sk.poly(), FheParams::SK_H);
}
