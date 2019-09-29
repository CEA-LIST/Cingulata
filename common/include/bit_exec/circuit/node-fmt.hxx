
namespace fmt {

template <>
struct formatter<cingulata::Node::NodeType> : formatter<string_view> {
  template <typename FormatContext>
  auto format(cingulata::Node::NodeType type, FormatContext &ctx) {
    string_view name = "UNKNOWN";
    switch (type) {
    case cingulata::Node::NodeType::INPUT:
      name = "INPUT";
      break;
    case cingulata::Node::NodeType::OUTPUT:
      name = "OUTPUT";
      break;
    case cingulata::Node::NodeType::GATE:
      name = "GATE";
      break;
    default:
      break;
    }
    return formatter<string_view>::format(name, ctx);
  }
};

template <>
struct formatter<cingulata::Node::GateType> : formatter<string_view> {
  template <typename FormatContext>
  auto format(cingulata::Node::GateType type, FormatContext &ctx) {
    string_view name = "UNKNOWN";
    switch (type) {
    case cingulata::Node::GateType::ZERO:
      name = "ZERO";
      break;
    case cingulata::Node::GateType::ONE:
      name = "ONE";
      break;
    case cingulata::Node::GateType::NOT:
      name = "NOT";
      break;
    case cingulata::Node::GateType::BUF:
      name = "BUF";
      break;
    case cingulata::Node::GateType::AND:
      name = "AND";
      break;
    case cingulata::Node::GateType::NAND:
      name = "NAND";
      break;
    case cingulata::Node::GateType::ANDNY:
      name = "ANDNY";
      break;
    case cingulata::Node::GateType::ANDYN:
      name = "ANDYN";
      break;
    case cingulata::Node::GateType::OR:
      name = "OR";
      break;
    case cingulata::Node::GateType::NOR:
      name = "NOR";
      break;
    case cingulata::Node::GateType::ORNY:
      name = "ORNY";
      break;
    case cingulata::Node::GateType::ORYN:
      name = "ORYN";
      break;
    case cingulata::Node::GateType::XOR:
      name = "XOR";
      break;
    case cingulata::Node::GateType::XNOR:
      name = "XNOR";
      break;
    case cingulata::Node::GateType::MUX:
      name = "MUX";
      break;
    default:
      break;
    }
    return formatter<string_view>::format(name, ctx);
  }
};

template <> struct formatter<cingulata::Node> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const cingulata::Node &node, FormatContext &ctx) {
    return format_to(ctx.out(), "{} (id: {}, inp: [{}], outs: [{}], {})",
                     node.get_type(), node.get_id(), join(node.get_preds(), ", "),
                     join(node.get_succs(), ", "),
                     node.get_gate_type());
  }
};
} // namespace fmt
