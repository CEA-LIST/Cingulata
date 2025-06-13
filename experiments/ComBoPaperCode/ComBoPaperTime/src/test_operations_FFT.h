#ifndef _TEST_OPERATIONS_FFT_H
#define _TEST_OPERATIONS_FFT_H

/** includes **/

#include "tfhe.h"

/** prototypes **/
void test_time_paper(int n, int N, int l, int t, int Bgbit, int basebit, double stddev_TLWE, double stddev_TRLWE, int mod);

#endif
