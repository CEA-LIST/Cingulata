#include "gtest/gtest.h"

#include <ci_context.hxx>
#include "bit_exec_plain.hxx"

using namespace cingulata;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  CiContext::set_bit_exec(new BitExecFake());

  return RUN_ALL_TESTS();
}
