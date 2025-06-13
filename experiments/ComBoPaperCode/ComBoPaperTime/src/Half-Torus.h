#ifndef _HT_H
#define _HT_H

#include "tfhe.h"


void id_interm(TLweSample *result,
    const LweBootstrappingKeyFFT *bk,
    int mod,
    const LweSample *x);

void id_HT(LweSample* res,
	LweSample* x,
	const LweBootstrappingKeyFFT *bk,
	int mod);

#endif
