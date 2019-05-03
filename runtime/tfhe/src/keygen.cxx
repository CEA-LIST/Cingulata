/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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
 * Application for generation of TFHE lib secret and public keys.
 * @note       for instant
 */

#include <tfhe.h>

#include <string>
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

struct Options {
  string KeyFilePrefix;
  bool strOutput;
};

Options parseArgs(int argc, char** argv) {
  Options options;
  string prefix;

  po::options_description config("Options");
  config.add_options()
      ("key-file-prefix", po::value<string>(&options.KeyFilePrefix)->default_value("tfhe"), "Prefix for key files")
      ("help,h", "produces this message")
  ;

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(config)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Generate TFHE secret and public key file(s)" << endl;
      cout << config << endl;
      exit(0);
    }

    po::notify(vm);
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

int main(int argc, char** argv) {
  Options options = parseArgs(argc, argv);

  // generate params
  TFheGateBootstrappingParameterSet *params =
      new_default_gate_bootstrapping_parameters(100);

  // generate secret key
  TFheGateBootstrappingSecretKeySet *sk =
      new_random_gate_bootstrapping_secret_keyset(params);

  // write secret key to file
  ofstream file_sk(options.KeyFilePrefix + ".sk");
  if (file_sk.is_open()) {
    export_tfheGateBootstrappingSecretKeySet_toStream(file_sk, sk);
    file_sk.close();
  }

  // get public key part
  const TFheGateBootstrappingCloudKeySet* pk = &(sk->cloud);

  // write public key to file
  ofstream file_pk(options.KeyFilePrefix + ".pk");
  if (file_pk.is_open()) {
    export_tfheGateBootstrappingCloudKeySet_toStream(file_pk, pk);
    file_pk.close();
  }
}
