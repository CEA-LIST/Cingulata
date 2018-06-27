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
 * @file generate_keys.cxx
 * @brief Main application for generation of secret, public and evaluation keys
 */

#include "fv.hxx"

#include <string>
#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

struct Options {
  string FheParamsFile;
  string KeyFilePrefix;
	rwBase base;
};

std::istream& operator>>(std::istream& is, rwBase& base)
{
	string tmp;
	is >> tmp;	

	if (tmp == "BIN") 
		base = BIN;			

	if (tmp == "B64") 
		base = B64;

	if (tmp == "B62") 
		base = B62;			
	
	return is;
}

Options parseArgs(int argc, char** argv) {
  Options options;
  string prefix;
  
  po::options_description config("Options");
  config.add_options()
      ("fhe-params", po::value<string>(&options.FheParamsFile)->default_value("fhe_params.xml"), "FHE parameters file")
      ("key-file-prefix", po::value<string>(&options.KeyFilePrefix)->default_value("fhe_key"), "Prefix for key files")
      ("help,h", "produce help message")
			("rw-base", po::value<rwBase>(&options.base)->default_value(BIN), "choose a base for encoding ciphertexts")
  ;

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(config)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Generate homomorphic cryptosystem keychain file(s)" << endl;
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

using namespace std;

int main(int argc, char** argv) {
  Options options = parseArgs(argc, argv);

  FheParams::readXml(options.FheParamsFile.c_str());

  KeyGen keygen;

  keygen.generateKeys();
	
  keygen.writeKeys(options.KeyFilePrefix, options.base);
}
