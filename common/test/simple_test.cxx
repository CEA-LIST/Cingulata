#include <bit_exec/tracker.hxx>

using namespace cingulata;


int main() {
  std::shared_ptr<IBitExec> be(new BitTracker());

  auto zero = be->encode();
  auto one = be->encrypt();
  auto inp2 = be->encrypt();
  auto a = be->op_and(one, inp2);
  be->write(a, "a");
  be->write(a, "b");
  be->write(inp2, "c");

  std::static_pointer_cast<BitTracker>(be)->export_blif(std::cout, "lala");
}
