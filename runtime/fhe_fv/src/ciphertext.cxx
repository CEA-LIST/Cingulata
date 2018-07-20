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

#include "fhe_params.hxx"
#include "ciphertext.hxx"

#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

/** @brief See header for a description
 */
void CipherText::relinearize(CipherText& ctr, const CipherText& EvalKey) {
  assert(ctr.size() == 3);

  /* Relinearization version 2 */
  CipherText rlk_cpy(EvalKey);
  CipherText::multiply_by_poly(rlk_cpy, ctr[2]);
  CipherText::multiply_round(rlk_cpy, 1, FheParams::P);
  CipherText::modulo(rlk_cpy, FheParams::Q);

  ctr.resize(2);

  CipherText::add(ctr, rlk_cpy);
  CipherText::modulo(ctr, FheParams::Q);
}

/** @brief See header for a description
 */
void CipherText::modulo(CipherText& ctr, const fmpz_t q) {
  for (unsigned int i = 0; i < ctr.size(); i++) {
    PolyRing::modulo(ctr[i], q);
  }
}

/** @brief See header for a description
 */
void CipherText::multiply_round(CipherText& ctr, const unsigned int t, const fmpz_t q) {
  for (unsigned int i = 0; i < ctr.size(); i++) {
    PolyRing::multiply_round(ctr[i], t, q);
  }
}

/** @brief See header for a description
 */
void CipherText::multiply_comp(CipherText& left_ctr, const CipherText& right_ctr) {
  assert(left_ctr.size() == right_ctr.size());
  
  for (unsigned int i = 0; i < left_ctr.size(); i++) {
    PolyRing::multiply(left_ctr[i], right_ctr[i]);
  }
}

/** @brief See header for a description
 */
CipherText::CipherText(unsigned int p_nrPolys):
    polysAllocated(true) {

  dataPoly.resize(p_nrPolys, NULL);
  for (unsigned int i = 0; i < dataPoly.size(); ++i) {
    dataPoly[i] = new PolyRing();
  }
}

/** @brief See header for a description
 */
CipherText::CipherText(const CipherText& ct):
    polysAllocated(true) {

  dataPoly.resize(ct.size(), NULL);
  for (unsigned int i = 0; i < dataPoly.size(); ++i) {
    dataPoly[i] = new PolyRing(ct[i]);
  }
}

/** @brief See header for a description
 */
CipherText::CipherText(const PolyRing& cp0):
    polysAllocated(true) {

  dataPoly.resize(1, NULL);
  dataPoly[0] = new PolyRing(cp0);
}

/** @brief See header for a description
 */
CipherText::CipherText(const PolyRing& cp0, const PolyRing& cp1):
    polysAllocated(true) {

  dataPoly.resize(2, NULL);
  dataPoly[0] = new PolyRing(cp0);
  dataPoly[1] = new PolyRing(cp1);
}

/** @brief See header for a description
 */
CipherText::CipherText(PolyRing* const cp0, PolyRing* const cp1):
    polysAllocated(false) {

  dataPoly.resize(2, NULL);
  dataPoly[0] = cp0;
  dataPoly[1] = cp1;
}

/** @brief See header for a description
 */
CipherText::~CipherText() {
  if (polysAllocated) {
    for (unsigned int i = 0; i < size(); i++) {
      if (dataPoly[i] != NULL) delete dataPoly[i];
    }
  }
}

/** @brief See header for a description
 */
void CipherText::add(CipherText& ct1, const CipherText& ct2) {
  if (ct1.size() < ct2.size()) {
    ct1.resize(ct2.size());
  }

  for (unsigned int i = 0; i < ct2.size(); i++) {
    PolyRing::add(ct1[i], ct2[i]);
  }

  CipherText::modulo(ct1, FheParams::Q);
}

/** @brief See header for a description
 */
void CipherText::sub(CipherText& ct1, const CipherText& ct2) {
  if (ct1.size() < ct2.size()) {
    ct1.resize(ct2.size());
  }

  for (unsigned int i = 0; i < ct1.size(); i++) {
    PolyRing::sub(ct1[i], ct2[i]);
  }

  CipherText::modulo(ct1, FheParams::Q);
}

/** @brief See header for a description
 */
void CipherText::multiply(CipherText& ct1, const CipherText& ct2, const CipherText& EvalKey) {
  /* Multiply ciphertexts */
  CipherText::multiply(ct1, ct2);

  /* relinearize degree 2 ciphertext if needed */
  if (ct1.size() == 3) {
    CipherText::relinearize(ct1, EvalKey);
  }
}

/** @brief See header for a description
 */
void CipherText::multiply(CipherText& ct1, const CipherText& ct2) {
  if (ct2.size() == 1) {
    CipherText::multiply_by_poly(ct1, ct2[0]);
  } 
  else if (ct2.size() >= 2) {
    CipherText ct1_cpy(ct1);

    /* multiply first ct2 polynomial */
    CipherText::multiply_by_poly(ct1, ct2[0]);

    /* initialize new ciphertext polynomials */
    ct1.resize(ct1.size() + ct2.size() - 1);

    /* multiply second to before last ct2 polynomials */
    for (unsigned int i = 1; i < ct2.size() - 1; ++i) {
      CipherText ct1_tmp(ct1_cpy);
      CipherText::multiply_by_poly(ct1_tmp, ct2[i]);
      for (unsigned int k = 0; k < ct1_tmp.size(); ++k) {
        PolyRing::add(ct1[k+i], ct1_tmp[k]);
      }      
    }

    /* multiply last ct2 polynomial */
    unsigned int i = ct2.size() - 1;
    CipherText::multiply_by_poly(ct1_cpy, ct2[i]);
    for (unsigned int k = 0; k < ct1_cpy.size(); ++k) {
      PolyRing::add(ct1[k+i], ct1_cpy[k]);
    }         
  }

  if (ct2.size() >= 1) {
    CipherText::multiply_round(ct1, FheParams::T, FheParams::Q);
    CipherText::modulo(ct1, FheParams::Q);     
  }
}

/** @brief See header for a description
 */
void CipherText::multiply_by_poly(CipherText& ct1, const PolyRing& p2) {
  for (unsigned int i = 0; i < ct1.size(); ++i) {
    PolyRing::multiply(ct1[i], p2);
  }
}

/** @brief See header for a description
 */
void CipherText::read(FILE* const stream, const rwBase binary) {
 	if (binary == B64) {
		char* buff;
  	fscanf(stream, "%ms", &buff);
  	int size = atoi(buff);
		free(buff);
		if (size) {
 			this->resize(size);
			for (unsigned int i = 0; i < this->size(); i++) {
    		dataPoly[i]->read(stream, binary);
			}
		}
		/* add an error message when size is 0 */
	}
	else {
		fmpz_t size_fmpz;
  	fmpz_init(size_fmpz);

  	PolyRing::read_fmpz(size_fmpz, stream, binary);
  	unsigned int size = fmpz_get_ui(size_fmpz);

  	this->resize(size);
  	for (unsigned int i = 0; i < this->size(); i++) {
    	dataPoly[i]->read(stream, binary);
  	}

  	fmpz_clear(size_fmpz);
	}	
}

/** @brief See header for a description
 */
void CipherText::read(const string& inFileName, const rwBase binary) {
  FILE* stream;
  stream = fopen(inFileName.c_str(), (binary == BIN) ? "rb" : "r");
  if (stream == NULL) {
    cerr << "ERROR: Ciphertext::read cannot open file '" << inFileName << "'" << endl;
    exit(-1);
  }

  read(stream, binary);
  fclose(stream);
}

/** @brief See header for a description
 */
void CipherText::write(FILE* const stream, const rwBase binary) const {
	if (binary == B64) {
 		stringstream ss;
		ss << this->size();
		string s = ss.str();
		fprintf(stream, "%s\n", s.c_str());
		for (unsigned int i = 0; i < this->size(); i++) 
    		dataPoly[i]->write(stream, binary);
	}
	else {
		fmpz_t size;
  	fmpz_init_set_ui(size, this->size());
  
 	 	PolyRing::write_fmpz(stream, size, binary);
 	 
  	for (unsigned int i = 0; i < this->size(); i++) {
    	dataPoly[i]->write(stream, binary);
  	}  

  	fmpz_clear(size);
	}
}

/** @brief See header for a description
 */
void CipherText::write(const string& outFileName, const rwBase binary) const {
  FILE* stream;

  stream = fopen(outFileName.c_str(), (binary == BIN) ? "wb" : "w");
  write(stream, binary);
  fclose(stream);
}


void CipherText::resize(const int newSize) {
  assert(polysAllocated);

  int prevSize = size();

  if (newSize < prevSize) {
    for (int i = newSize; i < prevSize; ++i) {
      if (dataPoly[i] != NULL) delete dataPoly[i];
    }
  } 

  dataPoly.resize(newSize, NULL);

  if (newSize > prevSize) {
    for (int i = prevSize; i < newSize; ++i) {
      dataPoly[i] = new PolyRing();
    }
  }
}
