#include <stdio.h>
#include <iostream>
#include "test_operations_FFT.h"
#include <limits>


using namespace std;

int main(int argc, char** argv)
{
	int n, N, l, t, Bgbit, basebit, mod;
	double stddevTLWE, stddevTRLWE;
	cout << "Enter n, N, l, t, Bgbit, basebit, stddevTLWE, stddevTRLWE, mod" << endl;
	cin >> n;
	cin >> N;
	cin >> l;
	cin >> t;
	cin >> Bgbit;
	cin >> basebit;
	cin >> stddevTLWE;
	cin >> stddevTRLWE;
	cin >> mod;
	test_time_paper(n, N, l, t, Bgbit, basebit, stddevTLWE, stddevTRLWE, mod);
      
    return(0);
}
