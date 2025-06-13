#ifndef _FDFB_FUNCTIONS_H
#define _FDFB_FUNCTIONS_H

/** includes **/
#include "tfhe.h"
#include "base_b_keyswitchkey.h"


void tfhe_min_sign_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void apply_TLwe_LUT(TLweSample *result,
	const TLweSample *LUT,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void prep_lut(IntPolynomial *result,
	IntPolynomial *lut);

void combine_LUT(TLweSample *res,
	IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	TLweSample *b,
	const TLweParams *params,
	int mod);

void op_heav_FFT(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void id_LUT_FDFB(IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	int mod);

void relu_LUT_FDFB(IntPolynomial *LUT1,
	IntPolynomial *LUT2,
	int mod);

void id_FDFB(LweSample *res,
	LweSample *x,
	const LweBootstrappingKeyFFT *bk,
	const BaseBKeySwitchKey *ks,
	int mod,
	const LweKey* lwekey);
	
void relu_FDFB(LweSample *res,
	LweSample *x,
	const LweBootstrappingKeyFFT *bk,
	const BaseBKeySwitchKey *ks,
	int mod,
	const LweKey* lwekey);

#endif
