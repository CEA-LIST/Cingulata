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

#include "blif_circuit.hxx"

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
namespace ba = boost::algorithm;

using namespace std;

/**
 * Mapping between BLIF truth tables and gate types
 */
unordered_map<string, GateType> truthTable2gate = {
  {"0;", GateType::CONST_0},
  {"1;", GateType::CONST_1},
  {"01 1;10 1;", GateType::XOR},
  {"10 1;01 1;", GateType::XOR},
  {"11 1;", GateType::AND},
  {"00 0;", GateType::OR},
  {"0 1;", GateType::NOT},
  {"1 1;", GateType::BUFF},
};

GateType parseTruthTableString(const string& ttToken) {
  GateType type = GateType::UNDEF;

  try {
    type = truthTable2gate.at(ttToken);
  } catch (out_of_range& exc) {
    throw runtime_error("ERROR: Unknown gate with truth table " + ttToken);
  }
  
  return type;
}

struct GateRaw {
  vector<string> inputs;
  string truthTable;
  string output;

  GateRaw(vector<string> opers, string tt):
    inputs(opers.begin(), opers.end() - 1),
    truthTable(tt),
    output(opers.back())
  {}
};

void ReadBlifFileRaw(string fn,
    vector<GateRaw>& circuit,
    vector<string>& inpNodes,
    vector<string>& outNodes) {
  string line;
  ifstream blifFile(fn.c_str());
  if (not blifFile.is_open()) {
    throw runtime_error("ERROR: Cannot open BLIF file: " + fn);
  }

  vector<string> operators;
  vector<string>* current = NULL;
  string truthTable;

  enum {
    DEFAULT,
    INPUT,
    OUTPUT,
    NAMES
  } state = DEFAULT;
  
  while (getline(blifFile, line)) {
    ba::trim(line);
    
    if (line.size() == 0 or line[0] == '#') continue;
    if (ba::starts_with(line, ".model")) continue;

    if (ba::starts_with(line, ".end")) break;

    if (ba::starts_with(line, ".inputs") or ba::starts_with(line, ".outputs") or ba::starts_with(line, ".names")) {
      if (state == NAMES) {
        circuit.push_back(GateRaw(operators, truthTable));
        operators.clear();
        truthTable = "";
        state = DEFAULT;
      }
      
      if (ba::starts_with(line, ".inputs")) {
        state = INPUT;
        current = &inpNodes;
      }
      else if (ba::starts_with(line, ".outputs")) {
        state = OUTPUT;
        current = &outNodes;
      }
      else if (ba::starts_with(line, ".names")) {
        state = NAMES;
        current = &operators;
      }

      ba::trim_if(line, ba::is_any_of("\\") or ba::is_space());
      vector<string> spLine;
      ba::split(spLine, line, ba::is_space(), ba::token_compress_on);

      current->insert(current->end(), spLine.begin() + 1, spLine.end());
    }
    else if (ba::starts_with(line, ".")) {
      throw runtime_error("ERROR: Unknown BLIF command " + line);
    }
    else if (state == INPUT or state == OUTPUT) {
      ba::trim_if(line, ba::is_any_of("\\") or ba::is_space());
      vector<string> spLine;
      ba::split(spLine, line, ba::is_space(), ba::token_compress_on);

      current->insert(current->end(), spLine.begin(), spLine.end());
    }
    else if (state == NAMES) {
      truthTable += line + ";";
    }
  }
  if (state == NAMES) {
    circuit.push_back(GateRaw(operators, truthTable));
  }
  blifFile.close();
}

Circuit ReadBlifFile(const string& fn) {
  vector<GateRaw> circuitRaw;
  vector<string> inpNodes, outNodes;
  ReadBlifFileRaw(fn, circuitRaw, inpNodes, outNodes);

  Circuit circuit;
  unordered_map<string, Circuit::vertex_descriptor> id2vertex;

  for (auto id: inpNodes) {
    id2vertex[id] = add_vertex(GateProperties(id, GateType::INPUT), circuit);
  }
  for (auto gateRaw: circuitRaw) {
    id2vertex[gateRaw.output] = add_vertex(GateProperties(gateRaw.output), circuit);
  }
  for (auto id: outNodes) {
    circuit[id2vertex.at(id)].isOutput = true;
  }

  for (auto gateRaw: circuitRaw) {
    Circuit::vertex_descriptor out = id2vertex.at(gateRaw.output);
  
    for (auto inp: gateRaw.inputs) {
      add_edge(id2vertex.at(inp), out, circuit);
    }

    circuit[out].type = parseTruthTableString(gateRaw.truthTable);
  }
  
  return circuit;
}

void UpdateCircuitWithClearInputs(Circuit& circuit, const unordered_map<string, bool>& clearInps) {
  if (clearInps.size() == 0) return;

  Circuit::vertex_iterator vi, vi_end;

  for (tie(vi, vi_end) = vertices(circuit); vi != vi_end; vi++) {
    if (clearInps.find(circuit[*vi].id) != clearInps.end()) {
      if (clearInps.at(circuit[*vi].id)) {
        circuit[*vi].type = GateType::CONST_1;
      } else {
        circuit[*vi].type = GateType::CONST_0;
      }
    }
  }
}

void UpdateCircuitWithClearInputs(Circuit& circuit, unsigned int offset, const std::vector<unsigned int>& clearInps)
{
	if (clearInps.size() == 0) return;
	
	unordered_map<string, bool> ci;

	for (unsigned int i = 0; i < clearInps.size(); i++) {
		string s = "i_" + to_string(i + offset + 2);
		ci.insert(make_pair(s , bool(clearInps[i])));
  }	
	
	UpdateCircuitWithClearInputs(circuit, ci);
}	
