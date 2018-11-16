#include <bit_exec/tracker.hxx>
#include <ci_bit.hxx>

using namespace cingulata;

int main() {
  IBitExec* be = new BitTracker();
  CiBit::set_bit_exec(be);

  CiBit a;
  a.encrypt();

  CiBit d;
  d.encrypt().set_name("lol");
  d = 1 ^ a;

  d.write();

  static_cast<BitTracker*>(be)->export_blif(std::cout, "lala");
}
