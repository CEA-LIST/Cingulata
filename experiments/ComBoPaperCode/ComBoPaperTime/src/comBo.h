#ifndef _COMBO_H
#define _COMBO_H

/** includes **/
#include "tfhe.h"

/** prototypes **/
void abs_interm(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);
    
void abs(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);
	
void tr_abs_interm(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);
    
void tr_abs(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void tr_id_interm(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);
    
void tr_id(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void div2_interm(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);
    
void div2(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void relu_comBo(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void relu_comBoMV(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void relu_comBoP(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

void luts_std_comp(IntPolynomial* lut_id_tr,
	IntPolynomial* lut_abs_tr, int mod);

void make_selector(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);
   
void calc_lut(TLweSample *result,
	IntPolynomial *lut,
	TLweSample *v);
	
#endif
