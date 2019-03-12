#include <bit_exec/tracker.hxx>
#include <ci_bit.hxx>
#include <ci_context.hxx>

using namespace cingulata;

int main() {
  CiContext::set_bit_exec(new BitTracker());

  CiBit a;
  a.encrypt();

  CiBit d;
  d.encrypt().set_name("lol");
  d = 1 ^ a;

  d.write();

  CiContext::get_bit_exec_t<BitTracker>()->export_blif(std::cout, "lala");
}
