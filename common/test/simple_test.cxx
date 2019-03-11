#include <bit_exec/tracker.hxx>
#include <ci_bit.hxx>

using namespace cingulata;

int main() {
  CiBit::set_bit_exec(new BitTracker());

  CiBit a;
  a.encrypt();

  CiBit d;
  d.encrypt().set_name("lol");
  d = 1 ^ a;

  d.write();

  CiBit::get_bit_exec<BitTracker>()->export_blif(std::cout, "lala");
}
