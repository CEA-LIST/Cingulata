/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.

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

#ifndef CINGULATA_BLIF_IO
#define CINGULATA_BLIF_IO

#include "circuit.hxx"

#include <iostream>
#include <unordered_map>
#include <vector>

namespace cingulata {

/* forward class declaration */
class Circuit;

class BlifOutput {
public:
  enum class GateReprType { LIB, LOGIC };

public:
  BlifOutput(const GateReprType p_gate_repr_type = GateReprType::LOGIC)
      : m_gate_repr_type(p_gate_repr_type) {}

  void write(std::ostream &, const Circuit &circuit);
  void write(const std::string &p_file_name, const Circuit &circuit);

private:
  std::vector<std::string> generate_names(const Circuit &circuit);

  GateReprType m_gate_repr_type;

  const char *const sm_inp_name_fmt = "i:%d";
  const char *const sm_out_name_fmt = "o:%d";
  const char *const sm_gate_name_fmt = "n%d";
};

class BlifInput {
public:
  BlifInput() = default;

  Circuit read(const std::string &p_file_name);
  Circuit read(std::istream &);

protected:
  Circuit circuit;
  std::unordered_map<std::string, Circuit::node_id_t> name_to_id;
  std::vector<std::string> output_names;

  void parse_inputs(const std::vector<std::string> &names);
  void parse_outputs(const std::vector<std::string> &names);
  void parse_lib_gate(const std::vector<std::string> &names);
  void parse_logit_gate(const std::vector<std::string> &names,
                        const std::string &truth_table);
};

} // namespace cingulata

#endif
