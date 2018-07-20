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
 * @file blif_circuit.hxx
 * @brief Blif format circuit reader
 */

#ifndef __BLIF_CIRCUIT_HXX__
#define __BLIF_CIRCUIT_HXX__

#include <string>
#include <vector>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>

/**
 * @brief Gate types
 */
enum class GateType {
  UNDEF     = 0,

  INPUT     = 1,
  CONST_0   = 2,
  CONST_1   = 3,
  AND       = 4,
  XOR       = 5,
  OR        = 6,
  NOT       = 7,
  BUFF      = 8
};

/**
 * @brief Gate properties structure
 * @details Structure for storing gate properties: id string, type and 
 *  whether is it output
 */
struct GateProperties {
  std::string id;
  GateType type;
  bool isOutput;

  GateProperties(const std::string& id_p = "", 
                  const GateType type_p = GateType::UNDEF, 
                  const bool isOutput_p = false): 
    id(id_p), type(type_p), isOutput(isOutput_p) {}
};

/**
 * Boost graph based on adjacency list for representing circuits
 */
typedef boost::adjacency_list<
   boost::vecS, boost::vecS, boost::bidirectionalS,
   GateProperties> Circuit;

/**
 * @brief Read blif file into boost graph
 * 
 * @param[in] fn input file name
 * @return boost graph
 */
Circuit ReadBlifFile(const std::string& fn);

/**
 * @brief Updates circuit with plain-text inputs
 * 
 * @param circuit to update
 * @param clearInps mapping between input name and constant value
 */
void UpdateCircuitWithClearInputs(Circuit& circuit, const std::unordered_map<std::string, bool>& clearInps);

/**
 * @brief Updates circuit with plain-text inputs
 * 
 * @param circuit to update
 * @offset defines the index of the clearInps
 * @param clearInps contain binary inputs for the circuit
 */
void UpdateCircuitWithClearInputs(Circuit& circuit, unsigned int offset, const std::vector<unsigned int>& clearInps);

#endif
