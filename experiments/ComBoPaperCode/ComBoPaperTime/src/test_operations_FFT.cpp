#include "base_b_keyswitch.h"
#include "base_b_keyswitchkey.h"
#include "comBo.h"
#include "FDFB_functions.h"
#include "Half-Torus.h"
#include "test_operations_FFT.h"
#include "tota_functions.h"
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <iostream>


using namespace std;
using namespace std::chrono;


void test_time_paper(int n, int N, int l, int t, int Bgbit, int basebit, double stddev_TLWE, double stddev_TRLWE, int mod)
{
	/********************
	 * Some parameters  *
	 ********************/
	
    // Bootstrapping parameters for 4 bits

    // Parameters
    int k = 1;
    
    const TLweParams* tlweparams = new_TLweParams(N, k, stddev_TRLWE, stddev_TRLWE);
    const LweParams * lweparams = new_LweParams(n, stddev_TLWE, stddev_TLWE);
    
    const TGswParams* tgswparams =  new_TGswParams(l, Bgbit, tlweparams);
    const TFheGateBootstrappingParameterSet* gateparams = new TFheGateBootstrappingParameterSet(t, basebit, lweparams, tgswparams);
        
   /************************************
    * Initialize TFHE key structures   *
    ************************************/
    
    TFheGateBootstrappingSecretKeySet* gk = new_random_gate_bootstrapping_secret_keyset(gateparams);
    const LweKey* lwekey = gk->lwe_key;
    const LweBootstrappingKeyFFT* bk = gk->cloud.bkFFT;
    BaseBKeySwitchKey* ks = new_BaseBKeySwitchKey(n, t, basebit, 1, tlweparams);
    BaseBExtra::CreateKeySwitchKey(ks, lwekey, &gk->tgsw_key->tlwe_key);
        
    /**************
     * Début code *
     **************/
    srand (time(NULL));
    int val= rand()%mod;
    cout << "rand = " << val << endl;
    LweSample* a = new_LweSample(lweparams);
    LweSample* s = new_LweSample(lweparams);
    LweSample* res = new_LweSample(lweparams);
    auto start_stamp = high_resolution_clock::now();
    auto end_stamp = high_resolution_clock::now();
    auto elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
		
	lweSymEncrypt(a, modSwitchToTorus32(val, 2*mod), stddev_TLWE, lwekey);
		
	start_stamp = high_resolution_clock::now();
	id_HT(res, a, bk, mod);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time HT = " << elapsed_time.count() << endl;
	
	cout << "id HT = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, 2*mod),2*mod) << endl;
	
	lweSymEncrypt(a, modSwitchToTorus32(val, mod), stddev_TLWE, lwekey);
		
	start_stamp = high_resolution_clock::now();
	id_FDFB(res, a, bk, ks, mod, lwekey);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time FDFB = " << elapsed_time.count() << endl;
	
	cout << "id FDFB = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
		
	start_stamp = high_resolution_clock::now();
	op_sign_FFT(s, bk, mod, a);
	id_TOTA(res, bk, mod, a, s);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time TOTA = " << elapsed_time.count() << endl;
	
	cout << "id TOTA = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
		
	start_stamp = high_resolution_clock::now();
	abs(res, a, bk, mod);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time abs = " << elapsed_time.count() << endl;
	
	cout << "abs = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
		
	start_stamp = high_resolution_clock::now();
	relu_comBo(res, a, bk, mod);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time relu = " << elapsed_time.count() << endl;
	
	cout << "relu = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
	
	lweSymEncrypt(a, modSwitchToTorus32(val, mod), stddev_TLWE, lwekey);
		
	start_stamp = high_resolution_clock::now();
	relu_comBoMV(res, a, bk, mod);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time relu with multival = " << elapsed_time.count() << endl;
	
	cout << "relu = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
		
	start_stamp = high_resolution_clock::now();
	relu_comBoP(res, a, bk, mod);
	end_stamp = high_resolution_clock::now();
	elapsed_time = duration_cast<duration<double>>(end_stamp - start_stamp);
	cout << "time relu P = " << elapsed_time.count() << endl;
	
	cout << "relu = " << modSwitchFromTorus32(lweSymDecrypt(res, lwekey, mod),mod) << endl;
	
    /*********
     * Free  *
     *********/
    delete_gate_bootstrapping_secret_keyset(gk);
    delete_LweSample(a);
    delete_LweSample(s);
    delete_LweSample(res);
}
