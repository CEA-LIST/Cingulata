#include <iostream>
#include "cingulata_tools.hxx"

using namespace std;

int main(int argc, char **argv)
{	
	vector<unsigned int> pt; 
	vector<CipherText> ct;
		
	/* set FV parameters and generate keys: sk, evk and pk */
	cout << "[KEYS] <1> set FHE keys" << endl; 
	setFheKeys("fhe_params.xml");
	cout <<  "[KEYS] </1>" << endl << endl;
	
	/* encrypt input data */
	cout << "[ENCRYPT] <2> encrypt input data" << endl;
	pt.resize(16, 0);
	pt[5] = pt[7] = pt[13] = pt[15] = 1;
	encryptFV("fhe_key.pk", pt, ct);
	cout << "[ENCRYPT] </2>" << endl << endl ;

	/* circuit evaluation */
	cout << "[EVALUATE] <3> run blif circuit on encrypted data" << endl;
	executeCircuitWithThreads("add_circuit-opt.blif", "fhe_key.evk", "fhe_key.pk", ct, 4);	
	cout << "[EVALUATE] </3>" << endl << endl;

	/* decrypt the results of circuit evaluation */
	cout << "[DECRYPT] <4> decrypt circuit output data" << endl;
	decryptFV("fhe_key.sk", ct, pt);

	for (unsigned int i = 0; i < pt.size(); i++)
		cout << pt[i]<< " ";
	cout << endl;
	cout << "[DECRYPT] </4>" << endl << endl;


	return 0;
}	
