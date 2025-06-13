#include "FDFB_functions.h"
#include "base_b_keyswitch.h"
#include <iostream>


/* namespaces */
using namespace std;

// Renvoie -sgn(x)/4*mod de l'élément nécessaire dans FDFB
void tfhe_min_sign_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x)
{
	const TGswParams *bk_params = bk->bk_params;
	const TLweParams *accum_params = bk->accum_params;
	const LweParams *in_params = bk->in_out_params;
	const int32_t N = accum_params->N;
	const int32_t Nx2 = 2 * N;
	const int32_t n = in_params->n;
	int mu = Nx2/mod;
	Torus32 test_coef = modSwitchToTorus32(4*mod - 1, 4*mod);

	TorusPolynomial *testvect = new_TorusPolynomial(N);
	TorusPolynomial *testvectbis = new_TorusPolynomial(N);
	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), Nx2)+mu/2)%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
	}
  
	//the initial testvec = [mu,mu,mu,...,mu]
	for (int32_t i = 0; i < N; i++)
		testvect->coefsT[i] = test_coef;
	if (barb!=0)
	{
		torusPolynomialMulByXai(testvectbis, Nx2-barb, testvect);
	}
	else
	{
		torusPolynomialCopy(testvectbis, testvect);
	}
	tLweNoiselessTrivial(result, testvectbis, accum_params);
	tfhe_blindRotate_FFT(result, bk->bkFFT, bara, n, bk_params);

	delete[] bara;
	delete_TorusPolynomial(testvect);
	delete_TorusPolynomial(testvectbis);
}


void apply_TLwe_LUT(TLweSample *result,
	const TLweSample *LUT,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x)
{
	const TGswParams *bk_params = bk->bk_params;
	const TLweParams *accum_params = bk->accum_params;
	const LweParams *in_params = bk->in_out_params;
	const int32_t N = accum_params->N;
	const int32_t Nx2 = 2 * N;
	const int32_t n = in_params->n;
	int mu = Nx2/mod;

	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), Nx2)+mu/2)%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
	}
  
	//the initial testvec = [mu,mu,mu,...,mu]
	if (barb!=0)
	{
		torusPolynomialMulByXai(result->a, Nx2-barb, LUT->a);
		torusPolynomialMulByXai(result->b, Nx2-barb, LUT->b);
	}
	else
	{
		tLweCopy(result, LUT, accum_params);
	}
	tfhe_blindRotate_FFT(result, bk->bkFFT, bara, n, bk_params);

	delete[] bara;
}


void prep_lut(IntPolynomial *result,
	IntPolynomial *lut)
{
	int i;
	
	result->coefs[0]=(lut->coefs[0]+lut->coefs[lut->N-1]);
	for (i = 1; i < lut->N; i++)
	{
		result->coefs[i]=(lut->coefs[i]-lut->coefs[i-1]);
	}
}

void combine_LUT(TLweSample *res,
	IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	TLweSample *b,
	const TLweParams *params,
	int mod)
{
	int N = LUT1->N;
	IntPolynomial *diff = new_IntPolynomial(N);
	IntPolynomial *prep_diff = new_IntPolynomial(N);
	
	int i;
	
	for (i = 0; i < N; i++)
	{
		diff->coefs[i] = LUT2->coefs[i]-LUT1->coefs[i];
	}
	prep_lut(prep_diff, diff);
	tLweClear(res, params);
	tLweAddMulRTo(res, prep_diff, b, params);
	
	for (i = 0; i < N; i++)
	{
		res->b->coefsT[i] += modSwitchToTorus32(LUT1->coefs[i]%mod,mod);
	}
	
	delete_IntPolynomial(diff);
	delete_IntPolynomial(prep_diff);
}

// renvoie 0 si positif ou 1/2*mod si negatif
void op_heav_FFT(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tfhe_min_sign_FFT(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	res->b += modSwitchToTorus32(1, 4*mod);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}



void id_LUT_FDFB(IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	int mod)
{
	int i, j;
	int N = LUT1->N;
	int N2_mod = 2*N/mod;
	
	for (i = 0; i < N; i++)
	{
		LUT1->coefs[i] = i/N2_mod;
		LUT2->coefs[i] = -(i/N2_mod + mod/2);
	}
	
	/*for (i = 0; i < mod/2; i++)
	{
		for (j = 0; j < N2_mod; j++)
		{
			LUT1->coefs[i*N2_mod + j] = i;
			LUT2->coefs[i*N2_mod + j] = -(i + mod/2);
		}
	}*/
}


void relu_LUT_FDFB(IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	int mod)
{
	int i, j;
	int N = LUT1->N;
	int N2_mod = 2*N/mod;
	
	for (i = 0; i < N; i++)
	{
		LUT1->coefs[i] = i/N2_mod;
		LUT2->coefs[i] = 0;
	}
	
	/*for (i = 0; i < mod/2; i++)
	{
		for (j = 0; j < N2_mod; j++)
		{
			LUT1->coefs[i*N2_mod + j] = i;
			LUT2->coefs[i*N2_mod + j] = 0;
		}
	}*/
}


void id_FDFB(LweSample *res,
	LweSample *x,
	const LweBootstrappingKeyFFT *bk,
	const BaseBKeySwitchKey *ks,
	int mod,
	const LweKey* lwekey)
{
	int N = bk->accum_params->N;
	IntPolynomial *LUT1 = new_IntPolynomial(N);
	IntPolynomial *LUT2 = new_IntPolynomial(N);
	TLweSample *LUT = new_TLweSample(bk->accum_params);
	TLweSample *b = new_TLweSample(bk->accum_params);
	LweSample *temp = new_LweSample(bk->in_out_params);
	LweSample *u = new_LweSample(bk->extract_params);
	
	id_LUT_FDFB(LUT1, LUT2, mod);
	op_heav_FFT(temp, bk, mod, x);
	BaseBExtra::KeySwitch(b, ks, temp);
	combine_LUT(LUT, LUT1, LUT2, b, bk->accum_params, mod);
	apply_TLwe_LUT(b, LUT, bk, mod, x);
	tLweExtractLweSample(u, b, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, u);
	
	delete_IntPolynomial(LUT1);
	delete_IntPolynomial(LUT2);
	delete_TLweSample(LUT);
	delete_TLweSample(b);
	delete_LweSample(u);
	delete_LweSample(temp);
}


void relu_FDFB(LweSample *res,
	LweSample *x,
	const LweBootstrappingKeyFFT *bk,
	const BaseBKeySwitchKey *ks,
	int mod,
	const LweKey* lwekey)
{
	int N = bk->accum_params->N;
	IntPolynomial *LUT1 = new_IntPolynomial(N);
	IntPolynomial *LUT2 = new_IntPolynomial(N);
	TLweSample *LUT = new_TLweSample(bk->accum_params);
	TLweSample *b = new_TLweSample(bk->accum_params);
	LweSample *temp = new_LweSample(bk->in_out_params);
	LweSample *u = new_LweSample(bk->extract_params);
	
	relu_LUT_FDFB(LUT1, LUT2, mod);
	op_heav_FFT(temp, bk, mod, x);
	BaseBExtra::KeySwitch(b, ks, temp);
	combine_LUT(LUT, LUT1, LUT2, b, bk->accum_params, mod);
	apply_TLwe_LUT(b, LUT, bk, mod, x);
	tLweExtractLweSample(u, b, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, u);
	
	delete_IntPolynomial(LUT1);
	delete_IntPolynomial(LUT2);
	delete_TLweSample(LUT);
	delete_TLweSample(b);
	delete_LweSample(u);
	delete_LweSample(temp);
}
