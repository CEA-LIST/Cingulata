#include "Half-Torus.h"
#include "comBo.h"
#include <thread>

/********************************************************************
 * The intermediary id function is in Half-Torus.h (id_HT) *
 ********************************************************************/

using namespace std;

void abs_interm(TLweSample *result,
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
	Torus32 coef_plus = modSwitchToTorus32(1, mod);
	Torus32 quarter = modSwitchToTorus32(1, 4);

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
		testvect->coefsT[i] = (i/mu)*coef_plus+quarter;
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

void abs(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	abs_interm(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	res->b -= modSwitchToTorus32(1, 4);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}

//Intermediary function for pseudo odd functions
void tr_id_interm(TLweSample *result,
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
	Torus32 coef_plus = modSwitchToTorus32(1, mod);
	Torus32 translate = modSwitchToTorus32(1, 2*mod);

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
		testvect->coefsT[i] = (i/mu)*coef_plus+translate;
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

void tr_id(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tr_id_interm(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	res->b -= modSwitchToTorus32(1, 2*mod);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}

//Intermediary function for pseudo even functions
void tr_abs_interm(TLweSample *result,
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
	Torus32 coef_plus = modSwitchToTorus32(1, mod);
	Torus32 translate = modSwitchToTorus32(1, 2*mod) + modSwitchToTorus32(1, 4);

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
		testvect->coefsT[i] = (i/mu)*coef_plus+translate;
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


void tr_abs(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	tr_abs_interm(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	res->b -= modSwitchToTorus32(1, 4)+modSwitchToTorus32(1, 2*mod);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}


void div2_interm(TLweSample *result,
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

void div2(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	TLweSample* u = new_TLweSample(bk->accum_params);
	LweSample* temp = new_LweSample(bk->extract_params);
	
	div2_interm(u, bk, mod, x);
	tLweExtractLweSample(temp, u, bk->extract_params,  bk->accum_params);
	lweKeySwitch(res, bk->ks, temp);
	
	delete_TLweSample(u);
	delete_LweSample(temp);
}


void luts_std_comp(IntPolynomial* lut_id_tr,
	IntPolynomial* lut_abs_tr, int mod)
{
	int i, j;
	int N = lut_id_tr->N;
	int N2_mod = 2*N/mod;
	
	lut_id_tr->coefs[0] = mod/2;
	for (i = 1; i < N2_mod; i++)
	{
		lut_id_tr->coefs[i] = 0;
	}
	for (i = 1; i < mod/2; i++)
	{
		lut_id_tr->coefs[i*N2_mod] = 1;
		for (j = 1; j < N2_mod; j++)
		{
			lut_id_tr->coefs[i*N2_mod+j] = 0;
		}
	}
	
	lut_abs_tr->coefs[0] = mod;
	for (i = 1; i < N2_mod; i++)
	{
		lut_abs_tr->coefs[i] = 0;
	}
	for (i = 1; i < mod/2; i++)
	{
		lut_abs_tr->coefs[i*N2_mod] = 1;
		for (j = 1; j < N2_mod; j++)
		{
			lut_abs_tr->coefs[i*N2_mod+j] = 0;
		}
	}	
}


void relu_comBoMV(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	const int32_t N = bk->accum_params->N;
	LweSample* temp = new_LweSample(bk->in_out_params);
	LweSample* temp2 = new_LweSample(bk->in_out_params);
	TLweSample* selector = new_TLweSample(bk->accum_params);
	TLweSample* tlwe_res = new_TLweSample(bk->accum_params);
	LweSample* u = new_LweSample(bk->extract_params);
	IntPolynomial* lut_id_tr = new_IntPolynomial(N);
	IntPolynomial* lut_id_quart_tr = new_IntPolynomial(N);
	
	// make luts
	
	luts_std_comp(lut_id_tr,lut_id_quart_tr, mod);
	
	// make selector of x
	make_selector(selector, bk, mod, x);
	
	// calc lut
	calc_lut(tlwe_res, lut_id_tr, selector);
	tLweExtractLweSample(u, tlwe_res, bk->extract_params, bk->accum_params);
	lweKeySwitch(res, bk->ks, u);
	res->b -= modSwitchToTorus32(1, 2*mod);
	div2(temp, res, bk, mod);
	
	calc_lut(tlwe_res, lut_id_quart_tr, selector);
	tLweExtractLweSample(u, tlwe_res, bk->extract_params, bk->accum_params);
	lweKeySwitch(temp2, bk->ks, u);
	temp2->b -= modSwitchToTorus32(1, 4)+modSwitchToTorus32(1, 2*mod);
	div2(res, temp2, bk, mod);
	
	lweAddTo(res, temp, bk->in_out_params);
	
	delete_LweSample(temp);
	delete_LweSample(temp2);
	delete_TLweSample(selector);
	delete_TLweSample(tlwe_res);
	delete_LweSample(u);
	delete_IntPolynomial(lut_id_tr);
	delete_IntPolynomial(lut_id_quart_tr);
}


void relu_comBo(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	LweSample* temp = new_LweSample(bk->in_out_params);
	LweSample* temp2 = new_LweSample(bk->in_out_params);
	
	tr_id(res, x, bk, mod);
	div2(temp, res, bk, mod);
	
	tr_abs(temp2, x, bk, mod);
	div2(res, temp2, bk, mod);
	lweAddTo(res, temp, bk->in_out_params);
	
	delete_LweSample(temp);
	delete_LweSample(temp2);
}


void relu_comBoP(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod)
{
	
	LweSample* temp = new_LweSample(bk->in_out_params);
	LweSample* temp2 = new_LweSample(bk->in_out_params);
	LweSample* temp3 = new_LweSample(bk->in_out_params);
	
	thread th1(tr_id, temp, x, bk, mod);
	thread th2(tr_abs, temp3, x, bk, mod);
	th1.join();
	th2.join();
	
	th1 = thread(div2, temp2, temp, bk, mod);
	th2 = thread(div2, res, temp3, bk, mod);
	th1.join();
	th2.join();
	
	lweAddTo(res, temp2, bk->in_out_params);
	
	delete_LweSample(temp);
	delete_LweSample(temp2);
	delete_LweSample(temp3);
}


void make_selector(TLweSample *result,
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
	Torus32 test_coef = modSwitchToTorus32(mu, 2*Nx2);

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


void calc_lut(TLweSample *result,
	IntPolynomial *lut,
	TLweSample *v)
{
	torusPolynomialMulR(result->a, lut, v->a);
	torusPolynomialMulR(result->b, lut, v->b);
}
