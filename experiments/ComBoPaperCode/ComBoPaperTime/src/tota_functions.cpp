#include "tota_functions.h"


/* namespaces */
using namespace std;

// Renvoie -sgn(x)*1/4 de l'élément nécessaire dans TOTA
void tfhe_op_sign_FFT(TLweSample *result,
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
	Torus32 test_coef = modSwitchToTorus32(3, 4);

	TorusPolynomial *testvect = new_TorusPolynomial(N);
	TorusPolynomial *testvectbis = new_TorusPolynomial(N);
	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), N)+mu/4)%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = modSwitchFromTorus32(x->a[i], N);
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

void tfhe_id_TOTA_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s)
{
	const TGswParams *bk_params = bk->bk_params;
	const TLweParams *accum_params = bk->accum_params;
	const LweParams *in_params = bk->in_out_params;
	const int32_t N = accum_params->N;
	const int32_t Nx2 = 2 * N;
	const int32_t n = in_params->n;
	int N_mod = N/mod;
	Torus32 test_coef = modSwitchToTorus32(1, mod);

	TorusPolynomial *testvect = new_TorusPolynomial(N);
	TorusPolynomial *testvectbis = new_TorusPolynomial(N);
	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), N)+N_mod/2+modSwitchFromTorus32((s->b), Nx2))%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = (modSwitchFromTorus32(x->a[i], N)+modSwitchFromTorus32(s->a[i], Nx2))%Nx2;
	}
  
	//the initial testvec = [mu,mu,mu,...,mu]
	for (int32_t i = 0; i < mod; i++)
	{
		for (int32_t j = 0; j < N_mod; j++)
		{
			testvect->coefsT[i*N_mod+j] = i*test_coef;
		}
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


void tfhe_relu_TOTA_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s)
{
	const TGswParams *bk_params = bk->bk_params;
	const TLweParams *accum_params = bk->accum_params;
	const LweParams *in_params = bk->in_out_params;
	const int32_t N = accum_params->N;
	const int32_t Nx2 = 2 * N;
	const int32_t n = in_params->n;
	int N_mod = N/mod;
	Torus32 test_coef = modSwitchToTorus32(1, mod);

	TorusPolynomial *testvect = new_TorusPolynomial(N);
	TorusPolynomial *testvectbis = new_TorusPolynomial(N);
	int32_t *bara = new int32_t[N];
	int32_t barb = (modSwitchFromTorus32((x->b), N)+N_mod/2+modSwitchFromTorus32((s->b), Nx2))%Nx2;
	for (int32_t i = 0; i < n; i++)
	{
		bara[i] = (modSwitchFromTorus32(x->a[i], N)+modSwitchFromTorus32(s->a[i], Nx2))%Nx2;
	}
  
	//the initial testvec = [mu,mu,mu,...,mu]
	for (int32_t i = 0; i < mod/2; i++)
	{
		for (int32_t j = 0; j < N_mod; j++)
		{
			testvect->coefsT[i*N_mod+j] = i*test_coef;
			testvect->coefsT[(i+mod/2)*N_mod+j] = 0;
		}
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


void op_sign_FFT(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tfhe_op_sign_FFT(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	res->b += modSwitchToTorus32(1, 4);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}

void id_TOTA(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tfhe_id_TOTA_FFT(u, bk, mod, x, s);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}

void relu_TOTA(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tfhe_relu_TOTA_FFT(u, bk, mod, x, s);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}
