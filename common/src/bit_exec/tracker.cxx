#include <bit_exec/tracker.hxx>

using namespace std;
using namespace cingulata;

// namespace BTI = cingulata::BitTrackerInternal;

enum class BitTrackerInternal::NodeType : uint8_t {
  UNKNOWN = 0,
  INPUT  =  1<<0,
  GATE   =  1<<1,
  OUTPUT =  1<<2
};

enum class BitTrackerInternal::GateType : uint8_t {
  UNKNOWN = 0,
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

  bool is_gate() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::GATE); }

  bool is_output() const { return static_cast<uint8_t>(type) & static_cast<uint8_t>(NodeType::OUTPUT); }

  const char* gate_type_str() const {
    static const char* GateType2Str[] = {
      "UNKNOWN",
      "NOT","BUF",
      "AND","NAND","ANDNY","ANDYN","OR","NOR","ORNY","ORYN","XOR","XNOR"};
    return GateType2Str[(uint8_t)gate_type];
  }

  NodeType type = NodeType::UNKNOWN;
  GateType gate_type = GateType::UNKNOWN;

  vector<ObjHandleT<Node>> inps;

  string name;
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
  ObjHandleT<BTI::Node> hdl = new_handle();
  hdl->type = BTI::NodeType::GATE;
  hdl->gate_type = gate_type;
  hdl->inps.assign(inps_p);
  gates.push_back(hdl);
  return hdl;
}

ObjHandle BitTracker::add_input(const string& name) {
  ObjHandleT<BTI::Node> hdl = new_handle();
  hdl->type = BTI::NodeType::INPUT;
  hdl->name = name;
  inputs.push_back(hdl);
  return hdl;
}

void BitTracker::make_output(const ObjHandleT<BTI::Node>& inp, const std::string& name) {
  ObjHandleT<BTI::Node> hdl = inp;
  if (hdl->is_input() or hdl->is_output()) {
    hdl = (ObjHandleT<BTI::Node>)add_gate(BTI::GateType::BUF, {hdl});
  }
  hdl->type = static_cast<BTI::NodeType>(static_cast<uint8_t>(hdl->type) | static_cast<uint8_t>(BTI::NodeType::OUTPUT));
  hdl->name = name;
  outputs.push_back(hdl);
}

ObjHandle BitTracker::encode(const IBitExec::bit_plain_t pt_val) {
  return add_input();
}

ObjHandle BitTracker::encrypt(const IBitExec::bit_plain_t pt_val) {
  return add_input();
}

IBitExec::bit_plain_t BitTracker::decrypt(const ObjHandle& hdl) {
  make_output(hdl);
  return 0;
}

ObjHandle BitTracker::read(const std::string& name) {
  return add_input(name);
}

void BitTracker::write(const ObjHandle& hdl, const std::string& name) {
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

void* BitTracker::new_obj() {
  return new BTI::Node();
}

void BitTracker::del_obj(void * obj) {
  delete (BTI::Node*)obj;
}

void BitTracker::export_blif(ostream& stream, const string& model_name) {
  stream << "# Circuit created by Cingulata" << endl;
  stream << ".model " << model_name << endl;

  stream << ".inputs ";
  uint cnt = 0;
  for (ObjHandleT<BTI::Node>& node: inputs) {
    if (node->name.empty()) node->name = "i_" + to_string(cnt++);
    stream << node->name << " ";
  }
  stream << endl;

  stream << ".outputs ";
  cnt = 0;
  for (ObjHandleT<BTI::Node>& node: outputs) {
    if (node->name.empty()) node->name = "o_" + to_string(cnt++);
    stream << node->name << " ";
  }
  stream << endl;

  cnt = 0;
  for (ObjHandleT<BTI::Node>& node: gates) {
    if (node->name.empty()) node->name = "n" + to_string(cnt++);

    stream << ".gate " << node->gate_type_str();
    char inp_name = 'A';
    for (const auto& inp: node->inps) {
      stream << " " << inp_name << "=" << inp->name;
      inp_name++;
    }
    stream << " Y=" << node->name << endl;
  }

  stream << ".end" << endl;
}
