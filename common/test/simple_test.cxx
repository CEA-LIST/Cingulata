#include <bit_exec/bit_tracker.hxx>

using namespace cingulata;


int main() {
  std::shared_ptr<IBitExec> be(new BitTracker());

  auto zero = be->encode(0);
  auto one = be->encrypt(1);
  auto inp2 = be->encrypt(1);
  auto a = be->op_mux(zero, one, inp2);
  be->write(a, "a");
  be->write(a, "b");
  be->write(inp2, "c");

  std::static_pointer_cast<BitTracker>(be)->export_blif(std::cout, "lala");
}
