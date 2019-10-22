/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

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

/**
 * @file dyn_omp.cxx
 * @brief Application for executing blif circuit files using homomorphic
 * operations
 */

#include "bfv_bit_exec.hxx"
#include "blif_circuit.hxx"
#include "homomorphic_executor.hxx"
#include "scheduler.hxx"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;
namespace po = boost::program_options;
namespace ba = boost::algorithm;

/* Available scheduler priorities */
enum class PriorityType {
  Topological,
  InverseTopological,
  Earliest,
  Latest,
  MaxOutDegree,
  MinOutDegree
};

/* Command line options structure */
struct Options {
  string FheParamsFile;
  string PublicKeyFile;
  string BlifFile;
  string ClearInputsFile;
  int nrThreads;
  bool verbose;
  bool stringOutput;
  PriorityType priority = PriorityType::Topological;

  static PriorityType parsePriority(const string &token) {
    return string2priority.at(token);
  }

  static string toString(const PriorityType &priority) {
    return priority2string.at(priority);
  }

  static vector<string> getAllowedPriorities() {
    vector<string> res;
    for (auto it : priority2string) {
      res.push_back(it.second);
    }
    return res;
  }

private:
  static map<PriorityType, string> priority2string;
  static map<string, PriorityType> string2priority;

  static class _init {
  public:
    _init() {
      priority2string[PriorityType::Topological] = "topo";
      priority2string[PriorityType::InverseTopological] = "inv-topo";
      priority2string[PriorityType::Earliest] = "earliest";
      priority2string[PriorityType::Latest] = "latest";
      priority2string[PriorityType::MaxOutDegree] = "max-out";
      priority2string[PriorityType::MinOutDegree] = "min-out";

      for (auto it : priority2string) {
        string2priority[it.second] = it.first;
      }
    }
  } _initializer;
};

map<PriorityType, string> Options::priority2string;
map<string, PriorityType> Options::string2priority;
Options::_init Options::_initializer;

istream &operator>>(istream &in, PriorityType &priority) {
  string token;
  in >> token;

  try {
    priority = Options::parsePriority(token);
  } catch (out_of_range &exc) {
    throw po::invalid_option_value(token);
  }

  return in;
}

ostream &operator<<(ostream &out, PriorityType &priority) {
  out << Options::toString(priority);
  return out;
}

Options parseArgs(int argc, char **argv) {
  Options options;
  vector<string> outputFileMessagePairs;
  string priorityHelp = "Priority function used for scheduling";
  priorityHelp +=
      " available options: " + ba::join(Options::getAllowedPriorities(), ", ");

  /* clang-format off */
  po::options_description config("Options");
  config.add_options()
      ("fhe-params", po::value<string>(&options.FheParamsFile)->default_value("fhe_params.xml"), "FHE parameters")
      ("public-key", po::value<string>(&options.PublicKeyFile)->default_value("fhe_key.pk"), "public key")
      ("strout", po::bool_switch(&options.stringOutput)->default_value(false), "output ciphertexts in string format")
      ("clear-inps", po::value<string>(&options.ClearInputsFile)->default_value(""), "clear inputs file")
      ("threads", po::value<int>(&options.nrThreads)->default_value(1), "number of parallel execution threads")
      ("priority", po::value<PriorityType>(&options.priority), priorityHelp.c_str())
      ("help,h", "produce help message")
      ("verbose,v", po::bool_switch(&options.verbose)->default_value(false), "enable verbosity")
  ;

  po::options_description hidden("Hidden");
  hidden.add_options()
      ("in_file", po::value<string>(&options.BlifFile), "")
  ;

  po::options_description all("All");
  all.add(config).add(hidden);

  po::positional_options_description p;
  p.add("in_file", -1);
  /* clang-format on */

  try {
    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(all).positional(p).run(),
        vm);

    if (vm.count("help")) {
      cout << "Homomorphically executes a BLIF circuit" << endl;
      cout << "Usage: " << argv[0] << " [options] <blif file>" << endl;
      cout << config << endl;
      exit(0);
    }

    po::notify(vm);

    if (options.BlifFile.size() == 0) {
      cerr << "Please specify an input BLIF file!" << endl;
      cerr << config << endl;
      exit(-1);
    }

  } catch (po::error &e) {
    cerr << "ERROR: " << e.what() << endl;
    cerr << config << endl;
    exit(-1);
  } catch (...) {
    cerr << "Something went wrong!!!" << endl;
    cerr << config << endl;
    exit(-1);
  }

  return options;
}

void readClearInputsFile(unordered_map<string, bool> &clearInps,
                         const Options &options) {
  ifstream file;
  file.open(options.ClearInputsFile.c_str());
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      ba::trim(line);

      if (line.size() == 0)
        continue;

      vector<string> spLine;
      ba::split(spLine, line, ba::is_space(), ba::token_compress_on);

      if (spLine.size() < 2) {
        cerr << "Line with only one token found when parsing clear inputs "
                "file!!!"
             << endl;
        exit(-1);
      }

      try {
        clearInps[spLine[0]] = boost::lexical_cast<bool>(spLine[1]);
      } catch (boost::bad_lexical_cast const &) {
        cerr << "Integer conversion error when parsing clear inputs file!!!"
             << endl;
        exit(-1);
      }
    }
    if (options.verbose) {
      cout << "Read " << clearInps.size() << " clear inputs from file '"
           << options.ClearInputsFile << "'" << endl;
    }
  }
  file.close();
}

int main(int argc, char **argv) {
  /* Parse command line options */
  Options options = parseArgs(argc, argv);

  /* Read FHE scheme parameters */
  auto context = make_shared<cingu::BfvBitExec::Context>(
      options.FheParamsFile, options.PublicKeyFile, cingu::BfvBitExec::Public);

  /* Create executor */
  cingu::BfvBitExec bfv_exec(context.get());

  if (options.verbose) {
    cout << "Reading circuit file " << options.BlifFile << endl;
  }
  /* Read blif file */
  Circuit circuit = ReadBlifFile(options.BlifFile);

  /* Read clear inputs file */
  unordered_map<string, bool> clearInps;
  if (options.ClearInputsFile.size() > 0) {
    readClearInputsFile(clearInps, options);
    UpdateCircuitWithClearInputs(circuit, clearInps);
  }

  if (options.verbose) {
    cout << "Creating scheduler and homomorphic execution environement" << endl;
  }

  /* Create homomorphic execution environment */
  HomomorphicExecutor *homExec = new HomomorphicExecutor(
      bfv_exec, circuit, options.verbose, options.stringOutput);

  /* Create priority object in function of cmd line parameter */
  Priority *priority = nullptr;
  switch (options.priority) {
  case PriorityType::Topological:
    priority = new PriorityTopological(circuit);
    break;
  case PriorityType::InverseTopological:
    priority = new PriorityInverseTopological(circuit);
    break;
  case PriorityType::Earliest:
    priority = new PriorityEarliest();
    break;
  case PriorityType::Latest:
    priority = new PriorityLatest();
    break;
  case PriorityType::MaxOutDegree:
    priority = new PriorityMaxOutDegree(circuit);
    break;
  case PriorityType::MinOutDegree:
    priority = new PriorityMinOutDegree(circuit);
    break;
  default:
    throw runtime_error("ERROR: priority object not created");
  }

  if (options.verbose) {
    cout << "Priority: " << Options::toString(options.priority) << endl;
  }

  /* Create scheduler */
  Scheduler *sched = new Scheduler(circuit, priority);

  function<void()> doWork = [homExec, sched, &circuit]() {
    Scheduler::Operation oper;
    do {
      oper = sched->next();

      if (oper.type == Scheduler::Operation::Type::Execute) {
        homExec->ExecuteGate(oper.node);
        sched->done(oper);
      } else if (oper.type == Scheduler::Operation::Type::Delete) {
        homExec->DeleteGateData(oper.node);
      }
    } while (oper.type != Scheduler::Operation::Type::Done);

    flint_cleanup();
  };

  if (options.verbose) {
    cout << "Start circuit execution" << endl;
  }

  steady_clock::time_point start = steady_clock::now();

  /* Create threads and start homomorphic executors */
  vector<thread> ths;
  for (int i = 0; i < options.nrThreads; i++) {
    ths.push_back(thread(doWork));
  }

  /* Start scheduling */
  sched->doSchedule();

  for (int i = 0; i < options.nrThreads; i++) {
    ths[i].join();
  }

  duration<double> execTime =
      duration_cast<duration<double>>(steady_clock::now() - start);
  cout << "Total execution real time " << execTime.count() << " seconds"
       << endl;
  homExec->printExecTime();

  delete sched;
  delete priority;
  delete homExec;

  return 0;
}
