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

#include <bit_exec/circuit/blif_io.hxx>
#include <bit_exec/circuit/circuit.hxx>
#include <bit_exec/decorator/attach.hxx>
#include <bit_exec/decorator/stat.hxx>
#include <bit_exec/decorator/stat_group.hxx>
#include <bit_exec/parallel/scheduler.hxx>

#include <bfv_bit_exec.hxx>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::chrono;
namespace po = boost::program_options;
namespace ba = boost::algorithm;

using namespace cingulata;
using namespace cingulata::parallel;

/* Command line options structure */
struct Options {
  string input_path;
  string output_path;
  string clear_inps_file;
  int nb_threads;
  bool verbose;

  string fhe_params_file;
  string public_key_file;
  bool str_inp;
  bool str_out;

  string blif_file;
};

Options parseArgs(int argc, char **argv) {
  Options options;
  vector<string> outputFileMessagePairs;

  po::options_description config("Options");
  config.add_options()(
      "input-path",
      po::value<string>(&options.input_path)->default_value("input"),
      "input ciphertexts path")(
      "output-path",
      po::value<string>(&options.output_path)->default_value("output"),
      "output ciphertexts path")(
      "clear-inps",
      po::value<string>(&options.clear_inps_file)->default_value(""),
      "clear inputs file")(
      "threads", po::value<int>(&options.nb_threads)->default_value(1),
      "number of parallel execution threads")("help,h", "produce help message")(
      "verbose,v", po::bool_switch(&options.verbose)->default_value(false),
      "enable verbosity");

  po::options_description config_fhe("BFV homomorphic scheme options");
  config.add_options()("fhe-params",
                       po::value<string>(&options.fhe_params_file)
                           ->default_value("fhe_params.xml"),
                       "BFV scheme parameters")(
      "public-key",
      po::value<string>(&options.public_key_file)->default_value("fhe_key.pk"),
      "public key")("strinp",
                    po::bool_switch(&options.str_inp)->default_value(false),
                    "read ciphertexts in string format")(
      "strout", po::bool_switch(&options.str_out)->default_value(false),
      "write ciphertexts in string format");

  po::options_description hidden("Hidden");
  hidden.add_options()("blif-file", po::value<string>(&options.blif_file), "");

  po::options_description all("All");
  all.add(config).add(config_fhe).add(hidden);

  po::positional_options_description p;
  p.add("blif-file", -1);

  try {
    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(all).positional(p).run(),
        vm);

    if (vm.count("help")) {
      fmt::print(
          "Parallel execution of BLIF circuits using homomorphic libraries\n"
          "Usage: {} [options] <blif file>\n{}\n",
          argv[0], config);
      exit(0);
    }

    po::notify(vm);

    if (options.blif_file.size() == 0) {
      fmt::print(stderr,
                 "Please specify an input BLIF file!\n"
                 "{}\n",
                 config);
      exit(-1);
    }

  } catch (po::error &e) {
    fmt::print(stderr, "ERROR: {}\n{}\n", e.what(), config);
    exit(-1);
  } catch (...) {
    fmt::print(stderr, "Something went wrong during argument parsing !!!\n{}\n",
               config);
    exit(-1);
  }

  return options;
}

unordered_map<string, bool> read_clear_file(const string &p_filename) {
  unordered_map<string, bool> clear_inps;

  ifstream file(p_filename.c_str());

  if (file.is_open()) {
    string line;

    while (getline(file, line)) {
      ba::trim(line);

      if (line.size() == 0)
        continue;

      vector<string> spLine;
      ba::split(spLine, line, ba::is_space(), ba::token_compress_on);

      if (spLine.size() < 2) {
        fmt::print(stderr, "Line with only one token found when parsing clear "
                           "inputs file!!!\n");
        exit(-1);
      }

      try {
        clear_inps[spLine[0]] = boost::lexical_cast<bool>(spLine[1]);
      } catch (boost::bad_lexical_cast const &) {
        fmt::print(
            stderr,
            "Integer conversion error when parsing clear inputs file!!!\n");
        exit(-1);
      }
    }

    file.close();
  } else {
  }

  return clear_inps;
}

int main(int argc, char **argv) {
  /* Parse command line options */
  Options options = parseArgs(argc, argv);

  /* Read blif file */
  auto circuit = BlifInput().read(options.blif_file);
  if (options.verbose) {
    fmt::print("Read BLIF file {}\n", options.blif_file);
  }

  /* Read clear inputs from file */
  unordered_map<string, bool> clear_inps;
  if (options.clear_inps_file.size() > 0) {
    clear_inps = read_clear_file(options.clear_inps_file);
    if (options.verbose) {
      fmt::print("Read {} clear inputs from file {}\n", clear_inps.size(),
                 options.clear_inps_file);
    }
  }

  if (options.verbose) {
    fmt::print("Creating workers and scheduler\n");
  }

  /* Read FHE scheme parameters */
  auto context = make_shared<BfvBitExec::Context>(
      options.fhe_params_file, options.public_key_file, BfvBitExec::Public);

  /* create bit executors */
  vector<shared_ptr<IBitExec>> bit_execs;
  for (int i = 0; i < options.nb_threads; ++i) {
    if (options.verbose) {
      bit_execs.push_back(
          make_shared<
              decorator::Attach<BfvBitExec, decorator::Stat<IBitExecSHE>>>(
              context.get()));
    } else {
      bit_execs.push_back(make_shared<BfvBitExec>(context.get()));
    }
  }

  Scheduler sched = Scheduler(bit_execs);

  if (options.verbose) {
    fmt::print("Start circuit execution\n");
  }

  steady_clock::time_point start = steady_clock::now();

  IBitExec *io_bit_exec = bit_execs.front().get();

  static_cast<BfvBitExec *>(io_bit_exec)->binary_input = not options.str_inp;
  static_cast<BfvBitExec *>(io_bit_exec)->binary_output = not options.str_out;

  /* Read encrypted inputs */
  if (options.verbose) {
    fmt::print("Read encrypted inputs...\n");
  }

  unordered_map<string, ObjHandle> inputs;
  for (const Node::id_t id : circuit.get_inputs()) {
    const string &name = circuit.get_name(id);
    if (clear_inps.find(name) == clear_inps.end()) {
      inputs[name] =
          io_bit_exec->read(fmt::format("{}/{}.ct", options.input_path, name));
    } else {
      inputs[name] = io_bit_exec->encode(clear_inps.at(name));
    }
  }

  if (options.verbose) {
    fmt::print("Execute...\n");
  }
  unordered_map<string, ObjHandle> outputs = sched.run(circuit, inputs);

  /* Write outputs */
  if (options.verbose) {
    fmt::print("Write encrypted inputs...\n");
  }

  for (const Node::id_t id : circuit.get_outputs()) {
    const string &name = circuit.get_name(id);
    assert(outputs.find(name) != outputs.end());
    io_bit_exec->write(outputs.at(name),
                       fmt::format("{}/{}.ct", options.output_path, name));
  }

  if (options.verbose) {
    duration<double> execTime =
        duration_cast<duration<double>>(steady_clock::now() - start);
    fmt::print("Total execution real time {} seconds\n", execTime.count());

    decorator::StatGroup<IBitExecSHE>(bit_execs).print();
  }

  return 0;
}
