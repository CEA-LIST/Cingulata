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
 * @file encrypt.cxx
 * @brief Utility for encrypting messages using homomorphic encryption
 */

#include "bfv_bit_exec.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
namespace po = boost::program_options;
namespace ba = boost::algorithm;
namespace cingu = cingulata;

struct Options {
  string FheParamsFile;
  string PublicKeyFile;
  string MessageFile;
  bool clear;
  unsigned int nbCoeffs;
  unsigned int nrThreads;
  vector< pair<string, vector<unsigned int> > > OutputFilesMessages;
  bool verbose;
};

/**
 * @brief Parses output files and messages from tokenized command line string
 * @details This methods takes tokenized command line arguments and groups them
 *  into pairs of "file name" and "messages".
 *  Messages split by space are polynomial coefficients.
 *
 * @param msgs_tokens tokenized command line
 * @param outputFilesMessages Output sfile and messages pairs
 */
void parseMessages(const vector<string>& msgs_tokens, vector< pair<string, vector<unsigned int> > >& outputFilesMessages) {
  vector<pair<string,string>> pairs;
  string fn, msgs;

  /* split message tokens in to file name and messages pairs */
  for (const string& token: msgs_tokens) {
    bool isFile = false;
    try {
      string tokenCpy(token);
      ba::trim_if(tokenCpy, ba::is_any_of(",") || ba::is_space());
      if (tokenCpy.size() > 0) {
        boost::lexical_cast<unsigned int>(tokenCpy);
      }
    } catch(const boost::bad_lexical_cast & e) {
      isFile = true;
    }

    if (isFile) {
      if (fn.size() > 0) {
        pairs.emplace_back(fn, msgs);
      }
      fn = token;
      msgs = "";
    } else {
      msgs += token + " ";
    }
  }
  if (fn.size() > 0) {
    pairs.emplace_back(fn, msgs);
  }

  /* parse to int tokens */
  for (unsigned int i = 0; i < pairs.size(); ++i) {
    tie(fn, msgs) = pairs[i];

    ba::trim(fn);
    ba::trim(msgs);

    vector<string> coef_msgs;
    ba::split(coef_msgs, msgs, ba::is_space(), ba::token_compress_on);

    vector<unsigned int> coefs;
    for (const string& msg: coef_msgs) {
      coefs.push_back(boost::lexical_cast<unsigned int>(msg));
    }
    outputFilesMessages.emplace_back(fn, coefs);
  }
}

/**
 * @brief Parse command line arguments
 */
Options parseArgs(int argc, char** argv) {
  Options options;
  vector<string> msgs_tokens;

  po::options_description config("Options");
  config.add_options()
      ("fhe-params", po::value<string>(&options.FheParamsFile)->default_value("fhe_params.xml"), "FHE parameters file")
      ("public-key", po::value<string>(&options.PublicKeyFile)->default_value("fhe_key.pk"), "Public key file")
      ("inp-file", po::value<string>(&options.MessageFile), "Read '<output file> [<message>]+' pairs from file")
      ("clear", po::bool_switch(&options.clear)->default_value(false), "'Encrypt' clear messages")
      ("threads", po::value<unsigned int>(&options.nrThreads)->default_value(1), "Number of parallel execution threads")
      ("help,h", "produce help message")
      ("verbose,v", po::bool_switch(&options.verbose)->default_value(false), "enable verbosity")
  ;

  po::options_description hidden("Hidden");
  hidden.add_options()
      ("file_messages_pairs", po::value< vector<string> >(&msgs_tokens)->required(), "")
  ;

  po::options_description all("All");
  all.add(config).add(hidden);

  po::positional_options_description p;
  p.add("file_messages_pairs", -1);

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(all)
                  .positional(p)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Encrypt message(s) and write them into ciphertext file(s),\n";
      cout << "\tpolynomial coefficients are separated by spaces.\n"
              "\tFor example message '1 1 0 1' means that polynomial\n"
              "\t'1 + X + X^3' should be encrypted.\n\n";

      cout << "Usage: " << argv[0] <<
              " [options] [<output file> [<message> ]+ ]+\n";
      cout << "\toutput file names cannot start by a digit\n\n";
      cout << "Examples:\n";
      cout << "\tExample 1 - encrypt single message per ciphertext:\n\t"
           << argv[0] << " [options] f0.ct 0 f1.ct 1 f2.ct 0\n";
      cout << "\tExample 2 - encrypt several messages into a ciphertext using"
              " coefficient packing:\n\t" << argv[0] <<
              " [options] f0.ct 0 f1.ct 1 0 1 f2.ct 1 1\n";

      cout << config << endl;
      exit(0);
    }

    po::notify(vm);

    if (vm.count("public-key") == 0) {
      cerr << "ERROR: No public key file specified!" << endl;
      cerr << config << endl;
      exit(-1);
    }

    /* Fill msgs_tokens from file */
    if (vm.count("inp-file") != 0) {
      ifstream file;
      file.open(options.MessageFile.c_str());
      if (not file.is_open()) {
        cerr << "ERROR: Cannot open '<output file> [<message>]+' pairs data file!" << endl;
        cerr << config << endl;
        exit(-1);
      }

      string token;
      while (true) {
        file >> token;
        if (file.eof()) break;
        msgs_tokens.push_back(token);
      }
    }

    /* Fill options.OutputFilesMessages from positional arguments */
    parseMessages(msgs_tokens, options.OutputFilesMessages);

    /* Validate messages to encrypt */
    options.nbCoeffs = 0;
    for (unsigned int i = 0; i < options.OutputFilesMessages.size(); ++i) {
      const string& out_fn = options.OutputFilesMessages[i].first;
      vector<unsigned int>& msgs = options.OutputFilesMessages[i].second;

      if (out_fn.empty()) {
        cerr << "ERROR: Empty output file name" << endl;
        cerr << config << endl;
        exit(-1);
      }

      if (msgs.empty()) {
        cerr << "ERROR: Please specify message(s) for output file '"
              << out_fn << "' !\n" << endl;
        cerr << config << endl;
        exit(-1);
      }

      if (options.nbCoeffs < msgs.size())
        options.nbCoeffs = msgs.size();
    }
  } catch (po::error& e) {
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


int main(int argc, char **argv) {
  Options options = parseArgs(argc, argv);

  cingu::BfvBitExec bfv_exec(options.FheParamsFile.c_str(), options.PublicKeyFile, cingu::BfvBitExec::Public);

  /* Validate options vs FHE parameters */
  unsigned int availNbCoefs = FheParams::D;
  if (options.nbCoeffs == 0) {
    options.nbCoeffs = availNbCoefs;
  }

  if (options.nbCoeffs > availNbCoefs) {
    cerr << "ERROR: Given number of coefficients ('nb_coef') " << options.nbCoeffs
      << " is bigger than the number of available coefficients "
      << availNbCoefs
      << " in ciphertext polynomial" << endl;
    exit(-1);
  }

  if (options.verbose) {
    cout << "Command line arguments:" << endl;
    cout << "FHE parameters file " << options.FheParamsFile << endl;
    cout << "Public key file " << options.PublicKeyFile << endl;
    if (options.nbCoeffs > 1) {
      cout << "Encrypt packed ciphertext: ";
      cout << "use coefficient packing" << endl;
    }
  }

  #pragma omp parallel for num_threads(options.nrThreads)
  for (unsigned int i = 0; i < options.OutputFilesMessages.size(); ++i) {
    const string& out_fn = options.OutputFilesMessages[i].first;
    const vector<unsigned int>& msgs = options.OutputFilesMessages[i].second;

    #pragma omp critical
    if (options.verbose) {
      printf("Encrypting message [");
      for (unsigned int i = 0; i < msgs.size(); ++i) {
        printf("%d%s", msgs[i], i < msgs.size()-1 ? " " : "");
      }
      printf("] into file '%s'\n", out_fn.c_str());
    }

    if (options.clear) {
      bfv_exec.write(bfv_exec.encode(msgs), out_fn);
    } else {
      bfv_exec.write(bfv_exec.encrypt(msgs), out_fn);
    }
  }

  return 0;
}

