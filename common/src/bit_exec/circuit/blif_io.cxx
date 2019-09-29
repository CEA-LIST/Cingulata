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

#include <spdlog/spdlog.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace cingulata;

namespace {

string gate_type_str(const Node::GateType gate_type) {
  switch (gate_type) {
  case Node::GateType::ZERO:
    return "ZERO";
  case Node::GateType::ONE:
    return "ONE";
  case Node::GateType::NOT:
    return "NOT";
  case Node::GateType::BUF:
    return "BUF";
  case Node::GateType::AND:
    return "AND";
  case Node::GateType::NAND:
    return "NAND";
  case Node::GateType::ANDNY:
    return "ANDNY";
  case Node::GateType::ANDYN:
    return "ANDYN";
  case Node::GateType::OR:
    return "OR";
  case Node::GateType::NOR:
    return "NOR";
  case Node::GateType::ORNY:
    return "ORNY";
  case Node::GateType::ORYN:
    return "ORYN";
  case Node::GateType::XOR:
    return "XOR";
  case Node::GateType::XNOR:
    return "XNOR";
  case Node::GateType::MUX:
    return "MUX";
  default:
    spdlog::warn("gate_cover_str - gate type unknown");
    return "UNKNOWN";
  }
}

string gate_cover_str(const Node::GateType gate_type) {
  switch (gate_type) {
  case Node::GateType::ZERO:
    return "";
  case Node::GateType::ONE:
    return "1";
  case Node::GateType::NOT:
    return "0 1";
  case Node::GateType::BUF:
    return "1 1";
  case Node::GateType::AND:
    return "11 1";
  case Node::GateType::NAND:
    return "11 0";
  case Node::GateType::ANDNY:
    return "01 1";
  case Node::GateType::ANDYN:
    return "10 1";
  case Node::GateType::OR:
    return "00 0";
  case Node::GateType::NOR:
    return "00 1";
  case Node::GateType::ORNY:
    return "10 0";
  case Node::GateType::ORYN:
    return "01 0";
  case Node::GateType::XOR:
    return "01 1\n10 1";
  case Node::GateType::XNOR:
    return "00 1\n11 1";
  case Node::GateType::MUX:
    return "010 1\n011 1\n101 1\n111 1";
  default:
    spdlog::warn("gate_cover_str - gate type unknown");
    return "UNKNOWN";
  }
}

void write_gate_lib_format(ostream &stream, const string &out_name,
                           const vector<string> &inp_names,
                           const Node::GateType gate_type) {

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
                             const Node::GateType gate_type) {

  stream << ".names ";
  for (const string &name : inp_names) {
    assert(not name.empty());
    stream << name << " ";
  }

  assert(not out_name.empty());
  stream << out_name << endl;

  const string cover = gate_cover_str(gate_type);
  if (not cover.empty())
    stream << gate_cover_str(gate_type) << endl;
}

void write_format_names(ostream &stream, const vector<string> &names,
                        uint start_idx = 0) {
  uint written_chars = start_idx; // nb of written chars per line
  for (const string &name : names) {
    assert(not name.empty());
    written_chars += name.size() + 1;
    if (written_chars >= 80) {
      stream << "\\" << endl;
      written_chars = name.size() + 1;
    }
    stream << name << " ";
  }
}

vector<string> get_names(const vector<string> &node_names,
                         const vector<Node::id_t> &ids) {
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
  write_format_names(stream, get_names(node_names, circuit.get_inputs()), 8);
  stream << endl;

  stream << ".outputs ";
  write_format_names(stream, get_names(node_names, circuit.get_outputs()), 9);
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
    if (node.is_gate()) {
      const vector<string> inp_names =
          get_names(node_names, circuit.get_preds(id));
      const string &out_name = node_names.at(id);

      (*write_gate)(stream, out_name, inp_names, node.get_gate_type());
    }
  }

  stream << ".end" << endl;
}

void BlifOutput::write(const string &file_name, const Circuit &circuit) {
  spdlog::debug("BlifInput::write(\"{}\",circuit) call", file_name);
  ofstream file(file_name);
  if (file.is_open()) {
    write(file, circuit);
  } else {
    spdlog::error("Unable to open blif file '{}' for write", file_name);
    exit(-1);
  }
}

vector<string> BlifOutput::generate_names(const Circuit &circuit) {
  constexpr static int m_name_size_max = 256;
  static char buf[m_name_size_max];

  unsigned inp_counter = 0;
  unsigned out_counter = 0;
  unsigned gate_counter = 0;

  vector<string> names(circuit.node_cnt());

  for (const Node::id_t id : circuit.get_inputs()) {
    string name = circuit.get_name(id);
    if (name.empty()) {
      std::snprintf(buf, m_name_size_max, sm_inp_name_fmt, inp_counter++);
      name = buf;
    }
    names[id] = name;
  }

  for (const Node::id_t id : circuit.get_outputs()) {
    string name = circuit.get_name(id);
    if (name.empty()) {
      std::snprintf(buf, m_name_size_max, sm_out_name_fmt, out_counter++);
      name = buf;
    }
    names[id] = name;

    // fill the only predecessors with output name
    const auto &pred_ids = circuit.get_preds(id);
    assert(pred_ids.size() == 1);
    names[pred_ids.front()] = name;
  }

  for (size_t id = 0; id < circuit.node_cnt(); ++id) {
    const Node &node = circuit.get_node(id);
    if (node.is_gate() and names[id].empty()) {
      std::snprintf(buf, m_name_size_max, sm_gate_name_fmt, gate_counter++);
      names[id] = buf;
    }
  }

  return names;
}

namespace {
void trim_and_remove_comments(string &str) {
  size_t t;

  // left trim
  t = str.find_first_not_of(" \t\n");
  if (t != string::npos)
    str.erase(0, t);

  // remove comments
  t = str.find_first_of("#");
  if (t != string::npos)
    str.erase(t);

  // right trim
  t = str.find_last_not_of(" \t\n");
  if (t != string::npos)
    str.erase(t + 1);
}

string get_line_cleared(istream &stream) {
  string line;
  getline(stream, line);
  trim_and_remove_comments(line);
  return line;
}

string get_multiline_cleared(istream &stream) {
  string line = get_line_cleared(stream);
  while (line.back() == '\\') {
    assert(stream);

    line.back() = ' ';
    line += get_line_cleared(stream);
  }
  return line;
}

vector<string> read_tokens(istream &iss) {
  vector<string> tokens;
  string tmp;
  while (iss >> tmp) {
    tokens.push_back(tmp);
  }
  return tokens;
}

string get_name_lib_token(const string &token) {
  size_t p = token.find('=');
  assert(p != string::npos);
  assert(p + 1 < token.size());
  return token.substr(p + 1);
}

Node::GateType get_lib_gate_type(const string &name) {
  static const unordered_map<string, Node::GateType> name_to_gate_type{
      {"ZERO", Node::GateType::ZERO},   {"ONE", Node::GateType::ONE},
      {"NOT", Node::GateType::NOT},     {"BUF", Node::GateType::BUF},
      {"AND", Node::GateType::AND},     {"NAND", Node::GateType::NAND},
      {"ANDNY", Node::GateType::ANDNY}, {"ANDYN", Node::GateType::ANDYN},
      {"OR", Node::GateType::OR},       {"NOR", Node::GateType::NOR},
      {"ORNY", Node::GateType::ORNY},   {"ORYN", Node::GateType::ORYN},
      {"XOR", Node::GateType::XOR},     {"XNOR", Node::GateType::XNOR},
      {"MUX", Node::GateType::MUX}};

  if (name_to_gate_type.find(name) != name_to_gate_type.end()) {
    return name_to_gate_type.at(name);
  } else {
    spdlog::warn("get_logic_gate_type - unknown lib gate type: {}", name);
    return Node::GateType::UNKNOWN;
  }
}

Node::GateType get_logic_gate_type(const string &tt) {
  static const unordered_map<string, Node::GateType> tt_to_gate_type{
      {"", Node::GateType::ZERO},
      {"0", Node::GateType::ZERO},
      {"1", Node::GateType::ONE},
      {"0 1", Node::GateType::NOT},
      {"1 1", Node::GateType::BUF},
      {"11 1", Node::GateType::AND},
      {"11 0", Node::GateType::NAND},
      {"01 1", Node::GateType::ANDNY},
      {"10 1", Node::GateType::ANDYN},
      {"00 0", Node::GateType::OR},
      {"00 1", Node::GateType::NOR},
      {"10 0", Node::GateType::ORNY},
      {"01 0", Node::GateType::ORYN},
      {"01 1\n10 1", Node::GateType::XOR},
      {"00 1\n11 1", Node::GateType::XNOR},
      {"010 1\n011 1\n101 1\n111 1", Node::GateType::MUX}};

  if (tt_to_gate_type.find(tt) != tt_to_gate_type.end()) {
    return tt_to_gate_type.at(tt);
  } else {
    spdlog::warn("get_logic_gate_type - unknown logic gate type:\n{}", tt);
    return Node::GateType::UNKNOWN;
  }
}
} // namespace

void BlifInput::parse_inputs(const vector<string> &names) {
  assert(names.size() > 0);

  for (const string &name : names) {
    Node::id_t id = circuit.add_input(name);

    assert((name_to_id.find(name) == name_to_id.end())); // no key repetition
    name_to_id.emplace(name, id);
  }
}
void BlifInput::parse_outputs(const vector<string> &names) {
  output_names.insert(output_names.end(), names.begin(), names.end());
  assert(unordered_set<string>(output_names.begin(), output_names.end())
             .size() == output_names.size()); // no key repetition
}

void BlifInput::parse_lib_gate(const vector<string> &names) {
  assert(names.size() > 1);

  const Node::GateType gate_type = get_lib_gate_type(names[0]);

  vector<Node::id_t> input_ids;
  for (unsigned i = 1; i < names.size() - 1; ++i) {
    const string &name = get_name_lib_token(names[i]);
    const Node::id_t id = name_to_id.at(name);
    input_ids.push_back(id);
  }
  Node::id_t id =
      circuit.add_gate(gate_type, input_ids.begin(), input_ids.end());

  const string &name = get_name_lib_token(names.back());

  assert((name_to_id.find(name) == name_to_id.end())); // no key repetition
  name_to_id.emplace(name, id);
}

void BlifInput::parse_logit_gate(const vector<string> &names, const string &truth_table) {
  assert(names.size() > 0);

  const Node::GateType gate_type = get_logic_gate_type(truth_table);

  vector<Node::id_t> input_ids;
  for (unsigned i = 0; i < names.size() - 1; ++i) {
    const string &name = names[i];
    const Node::id_t id = name_to_id.at(name);
    input_ids.push_back(id);
  }
  Node::id_t id =
      circuit.add_gate(gate_type, input_ids.begin(), input_ids.end());

  const string &name = names.back();

  assert((name_to_id.find(name) == name_to_id.end())); // no key repetition
  name_to_id.emplace(name, id);
}

Circuit BlifInput::read(const string &file_name) {
  spdlog::debug("BlifInput::read(\"{}\") call", file_name);

  ifstream file(file_name);
  if (file.is_open()) {
    return read(file);
  } else {
    spdlog::error("Unable to open blif file '{}' for read", file_name);
    exit(-1);
  }
}

Circuit BlifInput::read(istream &stream) {
  spdlog::debug("BlifInput::read(stream) call");

  circuit.clear();
  name_to_id.clear();
  output_names.clear();

  string line = get_multiline_cleared(stream);
  while (stream) {
    if (line.empty()) {
      line = get_multiline_cleared(stream);
      continue;
    }

    istringstream iss(line);

    string token;
    iss >> token;

    vector<string> tokens = read_tokens(iss);

    if (token == ".names") {
      // read truth table lines
      string truth_table;
      string line_tmp;
      while (stream) {
        line_tmp = get_multiline_cleared(stream);
        if (line_tmp.empty() or line_tmp.at(0) == '.')
          break;
        truth_table += line_tmp + '\n';
      }
      if (not truth_table.empty())
        truth_table.pop_back();

      parse_logit_gate(tokens, truth_table);
      line = line_tmp;
    } else {
      if (token == ".model") {
        assert(tokens.size() == 1 and "model line must have exactly 2 tokens");
        circuit.set_name(tokens.front());
      } else if (token == ".inputs") {
        parse_inputs(tokens);
      } else if (token == ".outputs") {
        parse_outputs(tokens);
      } else if (token == ".gate") {
        parse_lib_gate(tokens);
      } else if (token == ".end") {
        break;
      } else {
      }

      line = get_multiline_cleared(stream);
    }
  }

  // set output nodes
  for (const string &name : output_names) {
    Node::id_t nid = circuit.add_output(name_to_id.at(name), name);
    name_to_id[name] = nid;
  }

  return circuit;
}
