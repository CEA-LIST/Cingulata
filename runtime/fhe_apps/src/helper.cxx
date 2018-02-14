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
 * @file helper.cxx
 * @brief Helper application for building command line arguments for 'encrypt' binary
 *  and for transforming from decimal to binary and vice-versa
 */

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <iomanip>  
#include <iostream>  
#include <fstream>

using namespace std;
namespace po = boost::program_options;

struct Options {
  unsigned int BitCnt;
  bool FromBinary;
  bool MsbFirst;
  bool NoOutputFileName;
  bool File;
  string FilePrefix;
  string FileSuffix;
  int FileStartIdx;
  unsigned int IdxPlaces;
  vector<unsigned int> Messages;
};

Options parseArgs(int argc, char** argv) {
  Options options;
  
  po::options_description config("Options");
  config.add_options()
      ("bit-cnt", po::value<unsigned int>(&options.BitCnt)->default_value(1), "Number of bits to transform messages to")
      ("from-bin", po::bool_switch(&options.FromBinary)->default_value(false), "Transform messages from binary")
      ("msb-first", po::bool_switch(&options.MsbFirst)->default_value(false), "Most significant bit first")
      ("no-name", po::bool_switch(&options.NoOutputFileName)->default_value(false), "Do not print file names")      
      ("prefix", po::value<string>(&options.FilePrefix)->default_value("i_"), "File prefix")
      ("suffix", po::value<string>(&options.FileSuffix)->default_value(".ct"), "File suffix")
      ("start-idx", po::value<int>(&options.FileStartIdx)->default_value(2), "File index start value")
      ("idx-places", po::value<unsigned int>(&options.IdxPlaces)->default_value(0), "File index places")
      ("file", po::bool_switch(&options.File)->default_value(false), "Write result in plaintext/encoded_result (only if from-bin option is true)")
      ("help,h", "produce help message")
  ;

  po::options_description hidden("Hidden");
  hidden.add_options()
      ("messages", po::value< vector<unsigned int> >(&options.Messages), "")
  ; 

  po::options_description all("All");
  all.add(config).add(hidden);
  
  po::positional_options_description p;
  p.add("messages", -1);

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(all)
                  .positional(p)
                  .run(),
              vm);

    if (vm.count("help")) {
      cout << "Transforms message(s) from decimal to binary and vice-versa," << endl;
      cout << "\tUsed for generating command line arguments for 'encrypt' application" << endl;
      cout << "Usage: " << argv[0] <<
        " [options] [<message>]+" << endl;
      cout << "Example 1 - transform messages to binary\n\t" << argv[0] <<
        " --prefix f --bit-cnt 3 26 29" << endl;
      cout << "\tOutput: f2.ct 0 f3.ct 1 f4.ct 0 f5.ct 1 f6.ct 0 f7.ct 1" << endl;
      cout << "Example 2 - transform messages to binary\n\t" << argv[0] <<
        " --prefix f --no-name --bit-cnt 3 26 29" << endl;
      cout << "\tOutput: 0 1 0 1 0 1" << endl;
      cout << "Example 3 - transform messages to decimal\n\t" << argv[0] <<
        " --from-bin --bit-cnt 3 0 1 0 1 0 1" << endl;
      cout << "\tOutput: 2 5" << endl;
      cout << config << endl;
      exit(0);
    }
    
    po::notify(vm);

    if (options.Messages.empty()) {
      cerr << "Please specify at least one message!" << endl;
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

vector<unsigned int> toBinary(int num, unsigned int bitCnt, bool msbFirst) {
  vector<unsigned int> binForm;
  
  unsigned int i = 0;
  while (i < bitCnt) {
    binForm.push_back((num >> i++) & 1);
  };

  if (msbFirst) {
    std::reverse(binForm.begin(), binForm.end());
  }

  return binForm;
}

int fromBinary(vector<unsigned int> binForm, unsigned int bitCnt, bool msbFirst) {
  int num = 0;
  
  if (msbFirst) {
    std::reverse(binForm.begin(), binForm.end());
  }
  
  for (unsigned int i = 0; i < bitCnt and i < binForm.size(); i++) {
    num += (binForm[i] << i);
  }

  return num;
}

int main(int argc, char **argv) {
  Options options = parseArgs(argc, argv);

  if (options.FromBinary) {
		if (options.File)
		{
			ofstream file("plaintext/encoded_result", ios::out | ios::trunc); // open in write mode with content deleted
			if (file)
			{
				for (unsigned int i = 0; i < options.Messages.size(); i+= options.BitCnt) 
				{
					vector<unsigned int>::const_iterator first = options.Messages.begin() + i;
					vector<unsigned int>::const_iterator last = first + options.BitCnt;
					if (last > options.Messages.end()) 
					{
						last = options.Messages.end();
					}
					file << fromBinary(vector<unsigned int>(first, last), options.BitCnt, options.MsbFirst) << endl;
				}
				file.close();	    
			}
			else
			{
				cerr << "Impossible to open the file plaintext/encoded_result." << endl;
			} 
		}
		else
		{
    for (unsigned int i = 0; i < options.Messages.size(); i+= options.BitCnt) {
      vector<unsigned int>::const_iterator first = options.Messages.begin() + i;
      vector<unsigned int>::const_iterator last = first + options.BitCnt;
      if (last > options.Messages.end()) {
        last = options.Messages.end();
      }
      cout << fromBinary(vector<unsigned int>(first, last), options.BitCnt, options.MsbFirst) << " ";
    }
  }} else {
    for (unsigned int i = 0; i < options.Messages.size(); i++) {
      vector<unsigned int> temp = toBinary(options.Messages[i], options.BitCnt, options.MsbFirst);
      for (unsigned int j = 0; j < options.BitCnt; j++) {
        if (not options.NoOutputFileName) {
          cout << options.FilePrefix <<
              setw(options.IdxPlaces) << setfill('0') <<
              options.FileStartIdx + i*options.BitCnt + j <<
              options.FileSuffix << " ";
        }
        cout << temp[j] << " ";
      }
    }
  }
  cout << endl;
}
