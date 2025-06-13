#ifndef _TOTA_FUNCTIONS_H
#define _TOTA_FUNCTIONS_H

/** includes **/
#include "tfhe.h"


void tfhe_op_sign_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void tfhe_id_TOTA_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s);

void tfhe_relu_TOTA_FFT(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s);

void op_sign_FFT(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void id_TOTA(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s);

void relu_TOTA(LweSample* res,
	const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x,
    const LweSample *s);

#endif
