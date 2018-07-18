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

/**
 * @file homomorphic_executor.hxx
 * @brief class which tracks boolean circuit execution on HE data
 */

#ifndef __HOMOMORPHIC_EXECUTOR_HXX__
#define __HOMOMORPHIC_EXECUTOR_HXX__

#include "fv.hxx"
#include "blif_circuit.hxx"

#include <unordered_map>
#include <string>
#include <mutex>
#include <chrono>
#include <atomic>

class HomomorphicExecutor {
  private:
    /* Executed circuit */
    Circuit circuit;

    /* Homomorphic keys, ciphertexts, constants and parameters */
    KeysShare* keys;
    std::unordered_map<Circuit::vertex_descriptor, CipherText*> cipherTxts;
    CipherText* ct_const_0;
    CipherText* ct_const_1;

    /* Execution logs */
    std::unordered_map<std::string, double> execTime;
    std::unordered_map<std::string, int> execCnt;
    std::unordered_map<std::string, std::mutex*> execMtx;

    /* Number of allocated ciphertexts */
    std::atomic<int> allocatedCnt;
    int maxAllocatedCnt = 0;

    /* Verbose flag and logging mutex */
    bool verbose;
    std::mutex verboseMtx;

    std::string inpsDir = "input/";
    std::string outsDir = "output/";
    rwBase base;

    bool OutputToFile = true;

  private:
    /**
     * @brief Updates \c execTime and \c execCnt for metric \c name
     */
    void updateMeasures(const std::chrono::steady_clock::time_point& start, const std::string& name);

    /**
     * @brief Prints out \c gate properties, used for logging
     */
    void printGateInfo(const GateProperties& gate,
        const Circuit::vertex_descriptor pred1 = Circuit::null_vertex(),
        const Circuit::vertex_descriptor pred2 = Circuit::null_vertex());

    /**
     * @brief Allocates a new ciphertext
     */
    void Allocate(CipherText*& ct);

    /**
     * @brief Copies a ciphertext
     */
    void Copy(CipherText*& ct, const CipherText* const ct_cpy);

    /**
     * @brief Reads ciphertext \c ct from file \c fn
     */
    void Read(CipherText*& ct, const std::string& fn);

    /**
     * @brief Writes ciphertext \c ct to file \c fn
     */
    void Write(CipherText*& ct, const std::string& fn);

    /**
     * @brief Execute XOR gate
     * @details \code{ct_res = ct_n1 XOR ct_n2}
     */
    void ExecuteXOR(
      CipherText *&ct_res,
      const CipherText* const ct_n1,
      const CipherText* const ct_n2);

    /**
     * @brief Execute NOT gate
     * @details \code{ct_res = NOT ct_n1}
     */
    void ExecuteNOT(
      CipherText *&ct_res,
      const CipherText* const ct_n1);

    /**
     * @brief Execute AND gate
     * @details \code{ct_res = ct_n1 AND ct_n2}
     */
    void ExecuteAND(
      CipherText *&ct_res,
      const CipherText* const ct_n1,
      const CipherText* const ct_n2);

    /**
     * @brief Execute OR gate
     * @details \code{ct_res = ct_n1 OR ct_n2}
     */
    void ExecuteOR(
      CipherText *&ct_res,
      const CipherText* const ct_n1,
      const CipherText* const ct_n2);

    /**
     * @brief init function for HomomorphicExecutor constructors
     *
     */
    void init(const Circuit& circuit,
              const std::string& evalKeyFile, const std::string& publicKeyFile,
              const bool verbose_p, const rwBase base = BIN, const bool outToFile = true,
              const std::map<std::string, CipherText>& ct_in = std::map<std::string, CipherText>());

    public:
    /**
     * @brief Builds a homomorphic executor object
     *
     * @param[in] circuit boolean circuit to execute homomorphically
     * @param[in] evalKeyFile evaluation key file name
     * @param[in] publicKeyFile public key file name
     * @param[in] verbose_p verbose execution
     * @param[in] base for output encoding
     * @param[in] outToFile write output ciphertexts to .ct files
     * @param[in] ct_in input ciphertexts for the circuit
     */
    HomomorphicExecutor(const Circuit& circuit,
              const std::string& evalKeyFile, const std::string& publicKeyFile,
              const bool verbose_p, const rwBase base = BIN, const bool outToFile = true,
              const std::map<std::string, CipherText>& ct_in = std::map<std::string, CipherText>());

    /**
     * @brief Builds a homomorphic executor object
     *
     * @param[in] circuit boolean circuit to execute homomorphically
     * @param[in] evalKeyFile evaluation key file name
     * @param[in] publicKeyFile public key file name
     * @param[in] verbose_p verbose execution
     * @param[in] base for output encoding
     * @param[in] outToFile write output ciphertexts to .ct files
     * @param[in] ct_in input ciphertexts for the circuit
     */
    HomomorphicExecutor(const Circuit& circuit_p,
              const std::string& evalKeyFile, const std::string& publicKeyFile,
              const bool verbose_p, const rwBase base_p, const bool outToFile,
              const std::vector<CipherText>& ct_in);

    /**
     * @brief Destructs homomorphic executor object
     */
    ~HomomorphicExecutor();

    /**
     * @brief Delete data (ciphertext object) corresponding to gate \c idx
     */
    void DeleteGateData(const Circuit::vertex_descriptor idx);

    /**
     * @brief Executes gate \c idx
     */
    void ExecuteGate(const Circuit::vertex_descriptor idx);

    /**
     * @brief Recovers the encrypted outputs of the circuit evaluation
     */
    void GetOutputCiphertext(Circuit& circuit, std::map<std::string, CipherText*>& ct_out);

    /**
     * @brief Prints logged information about execution
     */
    void printExecTime();
};

#endif
