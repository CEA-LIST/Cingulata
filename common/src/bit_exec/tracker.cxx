#include <bit_exec/tracker.hxx>

#include <fstream>

using namespace std;
using namespace cingulata;

enum class BitTrackerInternal::NodeType : uint8_t {
  UNKNOWN     = 0,
  INPUT       = 1<<0,
  OUTPUT      = 1<<1,
  LOGIC_GATE  = 1<<2,
  LIB_GATE    = 1<<3
};

enum class BitTrackerInternal::GateType : uint8_t {
  UNKNOWN = 0,
  ZERO,
  ONE,
  NOT,
  BUF,
  AND,
  NAND,
  ANDNY,
  ANDYN,
  OR,
  NOR,
  ORNY,
  ORYN,
  XOR,
  XNOR,
  MUX
};

class BitTrackerInternal::Node {
public:
  Node() {
    inps.reserve(2);
  }

  bool is_input() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::INPUT); }

  bool is_logic_gate() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::LOGIC_GATE); }

  bool is_lib_gate() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::LIB_GATE); }

  bool is_output() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::OUTPUT); }

  const char* gate_type_str() const {
    static const char* GateType2Str[] = {
      "UNKNOWN",
      "ZERO","ONE",
      "NOT","BUF",
      "AND","NAND","ANDNY","ANDYN","OR","NOR","ORNY","ORYN","XOR","XNOR"};
    return GateType2Str[(uint8_t)gate_type];
  }

  const char* gate_cover_str() const {
    static const char* gate_cover[] = {
      "UNKNOWN", //UNKNOWN
      "0", //ZERO
      "1", //ONE
      "0 1", //NOT
      "1 1", //BUF
      "11 1", //AND
      "11 0", //NAND
      "01 1", //ANDNY
      "10 1", //ANDYN
      "00 0", //OR
      "00 1", //NOR
      "10 0", //ORNY
      "01 0", //ORYN
      "01 1\n10 1", //XOR
      "00 1\n11 1" //XNOR
    };
    return gate_cover[(uint8_t)gate_type];
  }

  void to_blif(ostream& stream) {
    if (is_logic_gate()) {
      to_blif_logic(stream);
    }
    else if (is_lib_gate()) {
      to_blif_lib(stream);
    }
  }

  NodeType type = NodeType::UNKNOWN;
  GateType gate_type = GateType::UNKNOWN;

  vector<ObjHandleT<Node>> inps;

  string name;

private:
  void to_blif_logic(ostream& stream) {
    stream << ".names ";
    for (const auto& inp: inps) {
      stream << inp->name << " ";
    }
    stream << name << endl;
    stream << gate_cover_str() << endl;
  }
  void to_blif_lib(ostream& stream) {
    stream << ".gate " << gate_type_str();
    char inp_name = 'A';
    for (const auto& inp: inps) {
      stream << " " << inp_name << "=" << inp->name;
      inp_name++;
    }
    stream << " Y=" << name << endl;
  }
};

BitTracker::~BitTracker() {
  reset();
}

void BitTracker::reset() {
  inputs.clear();
  outputs.clear();
  gates.clear();
}

ObjHandle BitTracker::add_gate(BTI::GateType gate_type, const initializer_list<ObjHandleT<BTI::Node>> inps_p) {
  ObjHandleT<BTI::Node> hdl = mm.new_handle();
  hdl->type = BTI::NodeType::LOGIC_GATE;
  hdl->gate_type = gate_type;
  hdl->inps.assign(inps_p);
  gates.push_back(hdl);
  return hdl;
}

ObjHandle BitTracker::add_input(const string& name) {
  ObjHandleT<BTI::Node> hdl = mm.new_handle();
  hdl->type = BTI::NodeType::INPUT;
  hdl->name = name;
  inputs.push_back(hdl);
  return hdl;
}

void BitTracker::make_output(const ObjHandleT<BTI::Node>& inp, const string& name) {
  ObjHandleT<BTI::Node> hdl = inp;
  if (hdl->is_input() or hdl->is_output()) {
    hdl = (ObjHandleT<BTI::Node>)add_gate(BTI::GateType::BUF, {hdl});
  }
  hdl->type = static_cast<BTI::NodeType>(static_cast<uint8_t>(hdl->type) | static_cast<uint8_t>(BTI::NodeType::OUTPUT));
  hdl->name = name;
  outputs.push_back(hdl);
}

ObjHandle BitTracker::encode(const bit_plain_t pt_val) {
  if (pt_val == 0) {
    return add_gate(BTI::GateType::ZERO, {});
  } else {
    return add_gate(BTI::GateType::ONE, {});
  }
}

ObjHandle BitTracker::encrypt(const bit_plain_t pt_val) {
  return add_input();
}

bit_plain_t BitTracker::decrypt(const ObjHandle& hdl) {
  make_output(hdl);
  return 0;
}

ObjHandle BitTracker::read(const string& name) {
  return add_input(name);
}

void BitTracker::write(const ObjHandle& hdl, const string& name) {
  make_output(hdl, name);
}

#define DEFINE_1_INP_OPER(OP_NAME, GATE_TYPE) \
ObjHandle BitTracker::OP_NAME(const ObjHandle& lhs) { \
  return add_gate(BTI::GateType::GATE_TYPE, {lhs}); \
}
#define DEFINE_2_INP_OPER(OP_NAME, GATE_TYPE) \
ObjHandle BitTracker::OP_NAME(const ObjHandle& lhs, const ObjHandle& rhs) { \
  return add_gate(BTI::GateType::GATE_TYPE, {lhs, rhs}); \
}

DEFINE_1_INP_OPER(op_not, NOT);
DEFINE_2_INP_OPER(op_and, AND);
DEFINE_2_INP_OPER(op_xor, XOR);
DEFINE_2_INP_OPER(op_nand, NAND);
DEFINE_2_INP_OPER(op_andyn, ANDYN);
DEFINE_2_INP_OPER(op_andny, ANDNY);
DEFINE_2_INP_OPER(op_or, OR);
DEFINE_2_INP_OPER(op_nor, NOR);
DEFINE_2_INP_OPER(op_oryn, ORYN);
DEFINE_2_INP_OPER(op_orny, ORNY);
DEFINE_2_INP_OPER(op_xnor, XNOR);

ObjHandle BitTracker::op_mux(const ObjHandle& cond, const ObjHandle& in1, const ObjHandle& in2) {
  return add_gate(BTI::GateType::MUX, {cond, in1, in2});
}

void BitTracker::export_blif(ostream& stream, const string& model_name) {
  stream << "# Circuit created by Cingulata" << endl;
  stream << ".model " << model_name << endl;

  stream << ".inputs ";
  uint cnt = 0;
  for (ObjHandleT<BTI::Node>& node: inputs) {
    if (node->name.empty()) node->name = to_string(cnt++);
    node->name = "i:" + node->name;
    stream << node->name << " ";
  }
  stream << endl;

  stream << ".outputs ";
  cnt = 0;
  for (ObjHandleT<BTI::Node>& node: outputs) {
    if (node->name.empty()) node->name = to_string(cnt++);
    node->name = "o:" + node->name;
    stream << node->name << " ";
  }
  stream << endl;

  cnt = 0;
  for (ObjHandleT<BTI::Node>& node: gates) {
    if (node->name.empty()) node->name = "n" + to_string(cnt++);
    node->to_blif(stream);
  }

  stream << ".end" << endl;
}

void BitTracker::export_blif(const string& file_name,
                             const string& model_name) {
  ofstream file(file_name);
  if (file.is_open()) {
    export_blif(file, model_name);
  } else {
    fprintf(stderr, "Error: Unable to open file '%s'\n", file_name.c_str());
  }
}
