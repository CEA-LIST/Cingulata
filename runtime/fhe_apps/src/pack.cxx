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
 * @file pack.cxx
 * @brief Utility for packing several ciphertext into single one using coefficient packing
 */


#include "fv.hxx"

#include <string>
#include <iostream>
#include <vector>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

struct Options {
  string EvalKeyFile;
  string FheParamsFile;
  vector<string> FileNames;
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

  po::options_description config("Options");
  config.add_options()
      ("fhe-params", po::value<string>(&options.FheParamsFile)->default_value("fhe_params.xml"), "FHE parameters file")
      ("eval-key", po::value<string>(&options.EvalKeyFile)->default_value("fhe_key.evk"), "Eval key file")
      ("help,h", "produce help message")  		
			("rw-base", po::value<rwBase>(&options.base)->default_value(BIN), "choose a base for encoding ciphertexts")
  ;

  po::options_description hidden("Hidden");
  hidden.add_options()
      ("files", po::value< vector<string> >(&options.FileNames), "")
  ;

  po::options_description all("All");
  all.add(config).add(hidden);

  po::positional_options_description p;
  p.add("files", -1);

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(all)
                  .positional(p)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Pack several ciphertexts into a single ciphertext using polynomial coefficient packing method" << endl;
      cout << "Usage: " << argv[0] <<
        " [options] [<input file>]+ <output file>" << endl;
      cout << "Example - pack messages a.ct, b.ct and c.ct into first, second and third polynomial coefficients of o.ct:\n\t" << argv[0] <<
        " [options] a.ct b.ct c.ct o.ct" << endl;
      cout << config << endl;
      exit(0);
    }

    po::notify(vm);
    
    if (options.FileNames.size() == 0) {
      cerr << "Please specify the output file!" << endl;
      cerr << config << endl;
      exit(-1);
    } else if (options.FileNames.size() == 1) {
      cerr << "Please specify at least one input file!" << endl;
      cerr << config << endl;
      exit(-1);
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

  FheParams::readXml(options.FheParamsFile.c_str());

  KeysShare keys;
  keys.readEvalKey(options.EvalKeyFile, options.base);

  CipherText ct_res;
  ct_res.read(options.FileNames[0], options.base);

  for (unsigned int i = 1; i < options.FileNames.size() - 1; i++) {
    string fileName = options.FileNames[i];

    CipherText ct;
    ct.read(fileName, options.base);

    PolyRing poly_x;
    poly_x.setCoeffUi(i, 1);

    CipherText poly_x_ct = EncDec::EncryptPoly(poly_x);
    CipherText::multiply(ct, poly_x_ct, *keys.EvalKey);
    CipherText::add(ct_res, ct);
  }

  	ct_res.write(options.FileNames.back(), options.base);

  return 0;
}
