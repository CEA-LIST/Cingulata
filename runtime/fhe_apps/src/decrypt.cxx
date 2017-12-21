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
 * @file decrypt.cxx
 * @brief Utility for decrypting messages using homomorphic encryption
 */

#include "fv.hxx"

#include <string>
#include <iostream>
#include <vector>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

struct Options {
  string FheParamsFile;
  string SecretKeyFile;
  bool signedMessage;
  bool noise;
  unsigned int nbCoeffs;
  unsigned  int nrThreads;
  vector<string> InputFiles;
  bool verbose;
};

Options parseArgs(int argc, char** argv) {
  Options options;

  po::options_description config("Options");
  config.add_options()
      ("fhe-params", po::value<string>(&options.FheParamsFile)->default_value("fhe_params.xml"), "FHE parameters file")
      ("secret-key", po::value<string>(&options.SecretKeyFile)->default_value("fhe_key.sk"), "Secret key file")
      ("signed,s", po::bool_switch(&options.signedMessage)->default_value(false), "Interpret decrypted messages as signed integers")

      ("nb_coef", po::value<unsigned  int>(&options.nbCoeffs)->default_value(1), "Number of polynomial coefficients to output (first one only by default). Use '0' for all coefficients.")

      ("noise", po::bool_switch(&options.noise)->default_value(false), "Output ciphertext noise (only works with verbose option on)")
      ("threads", po::value<unsigned  int>(&options.nrThreads)->default_value(1), "Number of parallel execution threads")
      ("help,h", "produce help message")
      ("verbose,v", po::bool_switch(&options.verbose)->default_value(false), "enable verbosity")
  ;

  po::options_description hidden("Hidden");
  hidden.add_options()
      ("input-file", po::value< vector<string> >(&options.InputFiles)->required(), "")
  ;

  po::options_description all("All");
  all.add(config).add(hidden);

  po::positional_options_description p;
  p.add("input-file", -1);

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(all)
                  .positional(p)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Decrypt ciphertext file(s)" << endl;
      cout << "Usage: " << argv[0] <<
        " [options] [<input file>]+" << endl;
      cout << "Example 1 - decrypt messages from f0.ct, f1.ct and f2.ct:\n\t" << argv[0] <<
        " [options] f0.ct f1.ct f2.ct" << endl;
      cout << "Example 2 - decrypt first 3 coefficients from f0.ct, f1.ct and f2.ct:\n\t" << argv[0] <<
        " [options] f0.ct f1.ct f2.ct --nb_coef 3" << endl;
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

int main(int argc, char **argv) {
  Options options = parseArgs(argc, argv);

  FheParams::readXml(options.FheParamsFile.c_str());

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
    cout << "Secret key file " << options.SecretKeyFile << endl;
    if (options.nbCoeffs > 1) {
      cout << "Decrypt packed ciphertext: ";
      cout << "use coefficient packing for the first ";
      cout << options.nbCoeffs << " coefficients" << endl;
    }
  }

  KeysAll keys;
  keys.readSecretKey(options.SecretKeyFile.c_str());

  #pragma omp parallel for ordered num_threads(options.nrThreads)
  for (unsigned int i = 0; i < options.InputFiles.size(); i++) {
    string fileName = options.InputFiles[i];

    CipherText ct;
    ct.read(fileName.c_str());

    PolyRing pTxtPoly = EncDec::DecryptPoly(ct, *keys.SecretKey);
    unsigned int noise;
    if (options.noise) {
      noise = EncDec::Noise(ct, *keys.SecretKey);
    }

    vector<int> msgs;
    for (unsigned int c = 0; c < options.nbCoeffs; ++c) {
      if (c < pTxtPoly.length()) {
        int msg = pTxtPoly.getCoeffUi(c);
        if (options.signedMessage and (unsigned int)msg > FheParams::T/2) msg -= FheParams::T;
        msgs.push_back(msg);
      } else {
        msgs.push_back(0);
      }
    }

    #pragma omp ordered
    {
      if (options.verbose) {
        cout << "Decrypting file " << fileName;
        if (options.noise) {
          cout << " - noise " << noise << "/" << FheParams::Q_bitsize;
        }
        cout << " - message [";
      }

      for (unsigned int i = 0; i < msgs.size(); ++i) {
        cout << msgs[i];
        if (i < msgs.size()-1)
          cout << " ";
      }
      if (options.verbose) {
        cout << "]";  
      }
      cout << endl;
    }
  }

  return 0;
}
