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

#ifndef BIT_EXEC_DECORATOR_STAT_GROUP
#define BIT_EXEC_DECORATOR_STAT_GROUP

#include <bit_exec/decorator/stat.hxx>

#include <cstdio>
#include <functional>

namespace cingulata {

class IBitExec;

namespace decorator {

template <typename bit_exec_interface_t> class StatGroup {
public:
  StatGroup(const std::shared_ptr<IBitExec> &p_bit_exec) {
    auto decorator =
        std::dynamic_pointer_cast<Stat<bit_exec_interface_t>>(p_bit_exec);
    assert(decorator);
    m_decorators.push_back(decorator);
  }

  StatGroup(const std::vector<std::shared_ptr<IBitExec>> &p_bit_execs) {
    for (auto bit_exec : p_bit_execs) {
      auto decorator =
          std::dynamic_pointer_cast<Stat<bit_exec_interface_t>>(bit_exec);
      assert(decorator);
      m_decorators.push_back(decorator);
    }
  }

  void print();

protected:
  void print_header();

  std::string build_line(unsigned (Stat<bit_exec_interface_t>::*cnt_fnc)()) {
    unsigned total_cnt;
    std::string buf;

    total_cnt = 0;
    buf.clear();
    for (const auto &decorator : m_decorators) {
      const unsigned cnt = (decorator.get()->*cnt_fnc)();
      total_cnt += cnt;
      buf += fmt::format("{:6} ", cnt);
    }

    if (m_decorators.size() > 1) {
      return fmt::format("{:6} | {}", total_cnt, buf);
    } else {
      return fmt::format("{:6} |", total_cnt);
    }
  }

  std::vector<std::shared_ptr<Stat<bit_exec_interface_t>>> m_decorators;
};

template <typename bit_exec_interface_t>
void StatGroup<bit_exec_interface_t>::print_header() {
  fmt::print("Execution statistics:\n");
  fmt::print("{:8} | {:6} |", "oper.", "total");
  if (m_decorators.size() > 1) {
    fmt::print(" {}", "per thread");
  }
  fmt::print("\n-------------------\n");
}

template <> void StatGroup<IBitExecFHE>::print() {
  print_header();

  fmt::print("{:8} | {}\n", "encode",
             build_line(&Stat<IBitExecFHE>::encode_cnt));
  fmt::print("{:8} | {}\n", "encrypt",
             build_line(&Stat<IBitExecFHE>::encrypt_cnt));
  fmt::print("{:8} | {}\n", "decrypt",
             build_line(&Stat<IBitExecFHE>::decrypt_cnt));
  fmt::print("{:8} | {}\n", "read", build_line(&Stat<IBitExecFHE>::read_cnt));
  fmt::print("{:8} | {}\n", "write", build_line(&Stat<IBitExecFHE>::write_cnt));
  fmt::print("-------------------\n");

  fmt::print("{:8} | {}\n", "not", build_line(&Stat<IBitExecFHE>::not_cnt));
  fmt::print("-------------------\n");

  fmt::print("{:8} | {}\n", "and", build_line(&Stat<IBitExecFHE>::and_cnt));
  fmt::print("{:8} | {}\n", "xor", build_line(&Stat<IBitExecFHE>::xor_cnt));
  fmt::print("{:8} | {}\n", "nand", build_line(&Stat<IBitExecFHE>::nand_cnt));
  fmt::print("{:8} | {}\n", "andyn", build_line(&Stat<IBitExecFHE>::andyn_cnt));
  fmt::print("{:8} | {}\n", "andny", build_line(&Stat<IBitExecFHE>::andny_cnt));
  fmt::print("{:8} | {}\n", "or", build_line(&Stat<IBitExecFHE>::or_cnt));
  fmt::print("{:8} | {}\n", "nor", build_line(&Stat<IBitExecFHE>::nor_cnt));
  fmt::print("{:8} | {}\n", "oryn", build_line(&Stat<IBitExecFHE>::oryn_cnt));
  fmt::print("{:8} | {}\n", "orny", build_line(&Stat<IBitExecFHE>::orny_cnt));
  fmt::print("{:8} | {}\n", "xnor", build_line(&Stat<IBitExecFHE>::xnor_cnt));
  fmt::print("{:8} | {}\n", "=total",
             build_line(&Stat<IBitExecFHE>::total_bin_op_cnt));
  fmt::print("-------------------\n");

  fmt::print("{:8} | {}\n", "mux", build_line(&Stat<IBitExecFHE>::mux_cnt));
  fmt::print("-------------------\n");
}

template <> void StatGroup<IBitExecSHE>::print() {
  print_header();

  fmt::print("{:8} | {}\n", "encode",
             build_line(&Stat<IBitExecSHE>::encode_cnt));
  fmt::print("{:8} | {}\n", "encrypt",
             build_line(&Stat<IBitExecSHE>::encrypt_cnt));
  fmt::print("{:8} | {}\n", "decrypt",
             build_line(&Stat<IBitExecSHE>::decrypt_cnt));
  fmt::print("{:8} | {}\n", "read", build_line(&Stat<IBitExecSHE>::read_cnt));
  fmt::print("{:8} | {}\n", "write", build_line(&Stat<IBitExecSHE>::write_cnt));
  fmt::print("-------------------\n");

  fmt::print("{:8} | {}\n", "and", build_line(&Stat<IBitExecSHE>::and_cnt));
  fmt::print("{:8} | {}\n", "xor", build_line(&Stat<IBitExecSHE>::xor_cnt));
  fmt::print("-------------------\n");
}

} // namespace decorator
} // namespace cingulata

#endif
