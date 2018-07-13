#include "cingulata_tools.hxx"

using namespace std;

/* FHE params and key setting functions */
void getFheParams(const char* const filename)
{
		FheParams::readXml(filename);
}

void setFheKeys(const char* const filename)
{
	getFheParams(filename);
	KeyGen keygen;
  keygen.generateKeys();
  keygen.writeKeys("fhe_key");
}

/* encryption functions */
void setClearInputFV(vector<unsigned int>& pt, vector<CipherText>& ct)
{
	ct.clear();
	for(unsigned int i = 0; i < pt.size(); i++) 
		ct.push_back(EncDec::Encrypt(pt[i]));
}

void encryptFV(const char* const pkFile, vector<unsigned int>& pt, vector<CipherText>& ct)
{
	KeysAll keys;
	keys.readPublicKey(pkFile);		
/*
#ifdef DEBUG
	cout << "[ENCRYPT] INPUT: " << endl;
	for (unsigned int i = 0; i < pt.size(); i++)
		cout << pt[i] << " ";
	cout << endl;
#endif
*/						
	ct.clear();
	for (unsigned int i = 0; i < pt.size(); i++)  
		ct.push_back(EncDec::Encrypt(pt[i], *keys.PublicKey));
/*
#ifdef DEBUG
	for (unsigned int i = 0; i < ct.size(); i++) {  
		cout << "[ENCRYPT] OUTPUT: " << i << endl;
		for(unsigned int j = 0; j < ct[i].size(); j++) {
			string out(PolyRing::encodeInBase64((ct[i])[j]));
			cout << out << endl << endl;
		}
	}	
#endif		
*/		
}	

/* decryption functions */
void decryptFV(const char* const skFile, vector<CipherText>& ct, vector<unsigned int>& pt)
{
	KeysAll keys;
	keys.readSecretKey(skFile);
	
	pt.clear();
	for (unsigned int i = 0; i < ct.size(); i++) { 
		PolyRing pr = EncDec::DecryptPoly(ct[i], *keys.SecretKey);
		if (pr.length()) 
			pt.push_back(pr.getCoeffUi(0));
		else	
			pt.push_back(0);
	}
}	

/* evaluation functions */
void executeCircuitWithThreads(const char* const blifFile, const char* const evkFile, const char* const pkFile, vector<CipherText>& ct, unsigned int thsNum)
{
	Circuit circuit = ReadBlifFile(blifFile);
	
	HomomorphicExecutor* homex = new HomomorphicExecutor(circuit, evkFile, pkFile, 0, BIN, false, ct);    

	Priority* priority = new PriorityTopological(circuit);
  
	Scheduler* sched = new Scheduler(circuit, priority);
	
 	function<void ()> doWork = [homex, sched, &circuit]() {
    		Scheduler::Operation oper;
    		do {
      			oper = sched->next();
      			if (oper.type == Scheduler::Operation::Type::Execute) {
							homex->ExecuteGate(oper.node);
							sched->done(oper);
      			} 
			else if (oper.type == Scheduler::Operation::Type::Delete) {
        			homex->DeleteGateData(oper.node);
      			}
    		} while (oper.type != Scheduler::Operation::Type::Done);

    		flint_cleanup();
  	};
	
  vector<thread> ths;
  for (unsigned int i = 0; i < thsNum; i++) 
   	ths.push_back(thread(doWork));

  sched->doSchedule();

  for (unsigned int i = 0; i < thsNum; i++) 
    ths[i].join();

	map<string, CipherText*> ct_out;
	homex->GetOutputCiphertext(circuit, ct_out);
	ct.clear();
	for (std::map<string,CipherText*>::iterator it=ct_out.begin(); it!=ct_out.end(); ++it) {
		ct.push_back(*ct_out[it->first]);
		delete ct_out[it->first];
	}
	
	delete sched;
	delete priority;
	delete homex;

	flint_cleanup();
}

