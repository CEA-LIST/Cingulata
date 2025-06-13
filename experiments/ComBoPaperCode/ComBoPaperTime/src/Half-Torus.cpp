#include "Half-Torus.h"
#include <iostream>

using namespace std;


void id_interm(TLweSample *result,
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
	int mu = N/mod;
	Torus32 coef_plus = modSwitchToTorus32(1, 2*mod);

	TorusPolynomial *testvect = new_TorusPolynomial(N);
	TorusPolynomial *testvectbis = new_TorusPolynomial(N);
	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), Nx2)+mu/2)%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = modSwitchFromTorus32(x->a[i], Nx2);
	}
  
	for (int32_t i = 0; i < N; i++)
	{
		testvect->coefsT[i] = (i/mu)*coef_plus;
	}
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

void id_HT(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	id_interm(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}
