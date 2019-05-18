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

#ifndef HOMOMORPHIC_EXECUTOR_HXX
#define HOMOMORPHIC_EXECUTOR_HXX

#include "bfv_bit_exec.hxx"
#include "blif_circuit.hxx"

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

namespace cingu = cingulata;

class HomomorphicExecutor {
private:
  /* Homomorphic keys, ciphertexts, constants and parameters */
  cingulata::BfvBitExec &m_exec;
  std::unordered_map<Circuit::vertex_descriptor, cingu::ObjHandle> cipherTxts;

  /* Executed circuit */
  Circuit circuit;

  /* Execution logs */
  std::unordered_map<std::string, double> execTime;
  std::unordered_map<std::string, int> execCnt;
  std::unordered_map<std::string, std::mutex *> execMtx;

  /* Number of allocated ciphertexts */
  std::atomic<int> allocatedCnt;
  int maxAllocatedCnt = 0;

  /* Verbose flag and logging mutex */
  bool verbose;
  std::mutex verboseMtx;

  std::string inpsDir = "input/";
  std::string outsDir = "output/";
  bool stringOutput;

private:
  /**
   * @brief Updates \c execTime and \c execCnt for metric \c name
   */
  void updateMeasures(const std::chrono::steady_clock::time_point &start,
                      const std::string &name);

  /**
   * @brief Prints out \c gate properties, used for logging
   */
  void printGateInfo(
      const GateProperties &gate,
      const Circuit::vertex_descriptor pred1 = Circuit::null_vertex(),
      const Circuit::vertex_descriptor pred2 = Circuit::null_vertex());

  /**
   * @brief Reads ciphertext \c ct from file \c fn
   */
  void Read(cingu::ObjHandle &ct, const std::string &fn);

  /**
   * @brief Writes ciphertext \c ct to file \c fn
   */
  void Write(cingu::ObjHandle &ct, const std::string &fn);

  /**
   * @brief Execute XOR gate
   * @details \code{ct_res = ct_n1 XOR ct_n2}
   */
  void ExecuteXOR(cingu::ObjHandle &ct_res, const cingu::ObjHandle &ct_n1,
                  const cingu::ObjHandle &ct_n2);

  /**
   * @brief Execute NOT gate
   * @details \code{ct_res = NOT ct_n1}
   */
  void ExecuteNOT(cingu::ObjHandle &ct_res, const cingu::ObjHandle &ct_n1);

  /**
   * @brief Execute AND gate
   * @details \code{ct_res = ct_n1 AND ct_n2}
   */
  void ExecuteAND(cingu::ObjHandle &ct_res, const cingu::ObjHandle &ct_n1,
                  const cingu::ObjHandle &ct_n2);

  /**
   * @brief Execute OR gate
   * @details \code{ct_res = ct_n1 OR ct_n2}
   */
  void ExecuteOR(cingu::ObjHandle &ct_res, const cingu::ObjHandle &ct_n1,
                 const cingu::ObjHandle &ct_n2);

public:
  /**
   * @brief      Builds a homomorphic executor object
   *
   * @param[in]  p_exec          B/FV bit executor
   * @param[in]  circuit_p       boolean circuit to execute homomorphically
   * @param[in]  verbose_p       verbose execution
   * @param[in]  stringOutput_p  write outputs in string format
   */
  HomomorphicExecutor(cingu::BfvBitExec &p_exec, const Circuit &circuit_p,
                      const bool verbose_p, const bool stringOutput_p);

  /**
   * @brief      Destructs homomorphic executor object
   */
  ~HomomorphicExecutor();

  /**
   * @brief      Delete data (ciphertext object) corresponding to gate @c idx
   *
   * @param[in]  idx   The index
   */
  void DeleteGateData(const Circuit::vertex_descriptor idx);

  /**
   * @brief      Executes gate @c idx
   *
   * @param[in]  idx   The index
   */
  void ExecuteGate(const Circuit::vertex_descriptor idx);

  /**
   * @brief      Prints logged information about execution
   */
  void printExecTime();
};

#endif
