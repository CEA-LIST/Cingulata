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

#include <bit_exec/circuit/blif_io.hxx>
#include <bit_exec/circuit/circuit.hxx>

#include <cassert>
#include <fstream>

using namespace std;
using namespace cingulata;

namespace {

const char *gate_type_str(const Circuit::GateType gate_type) {
  switch (gate_type) {
  case Circuit::GateType::ZERO:
    return "ZERO";
  case Circuit::GateType::ONE:
    return "ONE";
  case Circuit::GateType::NOT:
    return "NOT";
  case Circuit::GateType::BUF:
    return "BUF";
  case Circuit::GateType::AND:
    return "AND";
  case Circuit::GateType::NAND:
    return "NAND";
  case Circuit::GateType::ANDNY:
    return "ANDNY";
  case Circuit::GateType::ANDYN:
    return "ANDYN";
  case Circuit::GateType::OR:
    return "OR";
  case Circuit::GateType::NOR:
    return "NOR";
  case Circuit::GateType::ORNY:
    return "ORNY";
  case Circuit::GateType::ORYN:
    return "ORYN";
  case Circuit::GateType::XOR:
    return "XOR";
  case Circuit::GateType::XNOR:
    return "XNOR";
  case Circuit::GateType::MUX:
    return "MUX";
  default:
    return "UNKNOWN";
  }
}

const char *gate_cover_str(const Circuit::GateType gate_type) {
  switch (gate_type) {
  case Circuit::GateType::ZERO:
    return "0";
  case Circuit::GateType::ONE:
    return "1";
  case Circuit::GateType::NOT:
    return "0 1";
  case Circuit::GateType::BUF:
    return "1 1";
  case Circuit::GateType::AND:
    return "11 1";
  case Circuit::GateType::NAND:
    return "11 0";
  case Circuit::GateType::ANDNY:
    return "01 1";
  case Circuit::GateType::ANDYN:
    return "10 1";
  case Circuit::GateType::OR:
    return "00 0";
  case Circuit::GateType::NOR:
    return "00 1";
  case Circuit::GateType::ORNY:
    return "10 0";
  case Circuit::GateType::ORYN:
    return "01 0";
  case Circuit::GateType::XOR:
    return "01 1\n10 1";
  case Circuit::GateType::XNOR:
    return "00 1\n11 1";
  case Circuit::GateType::MUX:
    return "010 1\n011 1\n101 1\n111 1";
  default:
    return "UNKNOWN";
  }
}

void write_gate_lib_format(ostream &stream, const string &out_name,
                           const vector<string> &inp_names,
                           const Circuit::GateType gate_type) {

  stream << ".gate " << gate_type_str(gate_type);

  char tag = 'A';
  for (const string &name : inp_names) {
    assert(not name.empty());
    stream << " " << tag++ << "=" << name;
  }

  assert(tag < 'Y');
  assert(not out_name.empty());
  stream << " Y=" << out_name << endl;
}

void write_gate_logic_format(ostream &stream, const string &out_name,
                             const vector<string> &inp_names,
                             const Circuit::GateType gate_type) {
  stream << ".names ";
  for (const string &name : inp_names) {
    assert(not name.empty());
    stream << name << " ";
  }

  assert(not out_name.empty());
  stream << out_name << endl;

  stream << gate_cover_str(gate_type) << endl;
}

void write_format_names(ostream &stream, const vector<string> &names) {
  uint written_chars = 0; // nb of written chars per line
  for (const string &name : names) {
    assert(not name.empty());
    written_chars += name.size() + 1;
    if (written_chars >= 72) {
      stream << "\\" << endl;
      written_chars = name.size() + 1;
    }
    stream << name << " ";
  }
}

vector<string> get_names(const vector<string> &node_names,
                         const vector<Circuit::node_id_t> &ids) {
  vector<string> names;
  for (const auto id : ids)
    names.emplace_back(node_names.at(id));
  return names;
}

} // namespace

void BlifOutput::write(ostream &stream, const Circuit &circuit) {
  vector<string> node_names = generate_names(circuit);

  stream << "# Circuit created by Cingulata" << endl;
  stream << ".model " << circuit.get_name() << endl;

  stream << ".inputs ";
  write_format_names(stream, get_names(node_names, circuit.get_inputs()));
  stream << endl;

  stream << ".outputs ";
  write_format_names(stream, get_names(node_names, circuit.get_outputs()));
  stream << endl;

  /* get write gate function pointer depending on BlifOutput object parameters
   */
  auto write_gate = write_gate_logic_format;
  if (m_gate_repr_type == GateReprType::LIB) {
    write_gate = write_gate_lib_format;
  }

  /* write each gate node except inputs */
  for (size_t id = 0; id < circuit.node_cnt(); ++id) {
    const auto &node = circuit.get_node(id);
    if (node.is_input())
      continue;

    const vector<string> inp_names =
        get_names(node_names, circuit.get_preds(id));
    const string &out_name = node_names.at(id);

    (*write_gate)(stream, out_name, inp_names, node.gate_type());
  }

  stream << ".end" << endl;
}

void BlifOutput::write(const string &file_name, const Circuit &circuit) {
  ofstream file(file_name);
  if (file.is_open()) {
    write(file, circuit);
  } else {
    fprintf(stderr, "Error: Unable to open file '%s'\n", file_name.c_str());
  }
}

vector<string> BlifOutput::generate_names(const Circuit &circuit) {
  constexpr static int m_name_size_max = 256;
  static char buf[m_name_size_max];

  unsigned inp_counter = 0;
  unsigned out_counter = 0;
  unsigned gate_counter = 0;

  vector<string> names;

  for (const Circuit::Node &node : circuit.get_nodes()) {
    const string &name = node.get_name();
    if (name.empty()) {
      if (node.is_input()) {
        std::snprintf(buf, m_name_size_max, sm_inp_name_fmt, inp_counter++);
      } else if (node.is_output()) {
        std::snprintf(buf, m_name_size_max, sm_out_name_fmt, out_counter++);
      } else {
        std::snprintf(buf, m_name_size_max, sm_gate_name_fmt, gate_counter++);
      }
      names.emplace_back(buf);
    } else {
      names.emplace_back(name);
    }
  }

  return names;
}

Circuit BlifInput::read(const string &p_file_name) { return Circuit(); }

Circuit BlifInput::read(istream &stream) { return Circuit(); }
