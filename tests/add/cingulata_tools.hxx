#ifndef _CINGULATA_TOOLS_H
#define _CINGULATA_TOOLS_H

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <stdlib.h>

#include "fv.hxx"
#include "blif_circuit.hxx"
#include "scheduler.hxx"
#include "homomorphic_executor.hxx"
#include "priority.hxx"

//#define DEBUG 

/* get FV parameters from fhe_params.xml 
 */
void getFheParams(const char* const filename);

/* set public, secret and evaluation keys with respect to parameters from fhe_params.xml
 */
void setFheKeys(const char* const filename);

/* FV encryption function
 * @pkFile public key file fhe_key.pk
 * @pt table of binary plaintext inputs
 * @ct table of ciphertexts of pt inputs
 */
void encryptFV(const char* const pkFile, std::vector<unsigned int>& pt, std::vector<CipherText>& ct);

/* Format plaintexts as ciphertexts 
 * @pt table of binary plaintext inputs
 * @ct table of encoded plaintexts as ciphertexts
 */
void setClearInputFV(std::vector<unsigned int>& pt, std::vector<CipherText>& ct);

/* FV decryption function
 * @skFile secret key file fhe_key.sk
 * @ct table of ciphertexts for decryption
 * @pt table of binary output
 */
void decryptFV(const char* const skFile, std::vector<CipherText>& ct, std::vector<unsigned int>& pt);

/* Homomorphic evaluation functions 
 * @blifFile circuit optimized blif file
 * @evkFile evaluation key file fhe_key.evk
 * @pkFile public key file fhe_key.pk
 * @ct ciphertexts inputs/outputs from blif circuit
 * @thsNum thread number for boolean circuit evaluation
 */
void executeCircuitWithThreads(const char* const blifFile, const char* const evkFile, const char* const pkFile, std::vector<CipherText>& ct, unsigned int thsNum);

#endif
