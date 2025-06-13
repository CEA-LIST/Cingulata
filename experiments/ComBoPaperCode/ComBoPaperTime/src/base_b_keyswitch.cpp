#include "base_b_keyswitch.h"
#include <iostream>

/**
 * fills the KeySwitching key array
 * @param result The (n x t x base) array of samples.
 *    result[i][j][k] encodes k.s[i]/base^(j+1)
 * @param out_key The TLwe key to encode all the output samples
 * @param out_alpha The standard deviation of all output samples
 * @param in_key The (binary) input Lwe key
 * @param n The size of the input key
 * @param t The precision of the keyswitch (technically, 1/2.base^t)
 * @param basebit Log_2 of base
 * @param N degree of the polynomials of output samples
 * @param k number of polynomials of output samples
 */

void BaseBExtra::CreateKeySwitchKey_fromArray(
    TLweSample*** result,
    const TLweKey* out_key,
    const double out_alpha,
    const int* in_key,
    const int n,
    const int t,
    const int basebit,
    const int mod)
{
	int N = out_key->params->N;
	int N_B = N/mod;
	TorusPolynomial* x = new_TorusPolynomial(N);

    #pragma omp parallel for
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<t;j++)
        {
			for(int k = 0; k < mod; k++)
			{
				for(int ind = 0; ind < N; ind++)
				{
					if (ind >= k*N_B && ind < (k+1)*N_B)
					{
						x->coefsT[ind] = in_key[i]*(int32_t(1)<<(32-(j+1)*basebit));
						
					}
					else
					{
						x->coefsT[ind] = 0;
					}
				}
				tLweSymEncrypt(&result[i][j][k],x,out_alpha,out_key);
			}
        }
    }
}

/**
 * translates the message of the result sample by -sum(a[i].s[i]) where s is the secret
 * embedded in ks.
 * @param result the TLWE sample to translate by -sum(ai.si).
 * @param ks The (n x t x base) key switching key
 *    ks[i][j][k] encodes s[i]/base^(j+1).sum(X^r for r in [k*N_B,(k+1)*N_B[)
 * @param params The common TLwe parameters of ks and result
 * @param ai The input torus array
 * @param n The size of the input key
 * @param t The precision of the keyswitch (technically, 1/2.base^t)
 * @param basebit Log_2 of base
 */

void BaseBExtra::KeySwitchTranslate_fromArray(
    TLweSample* result,
    const TLweSample*** ks,
    const TLweParams* params,
    const int32_t* ai,
    const int n,
    const int t,
    const int basebit)
{
    const int base=1<<basebit;
    const int32_t prec_offset=int32_t(1)<<(32-(1+basebit*t)); //precision
    const int mask=base-1;


    for (int i = 0; i < n; i++)
    {
        const int32_t aibar=ai[i]+prec_offset;
        for (int j = 0; j < t; j++)
        {
            const int32_t aij=(aibar>>(32-(j+1)*basebit)) & mask;
            tLweSubMulTo(result, aij, &ks[i][j][0] ,params);
        }
    }
}


void BaseBExtra::CreateKeySwitchKey(
    BaseBKeySwitchKey* result,
    const LweKey* in_key,
    const TLweKey* out_key)
{
    const int n=result->n;
    const int basebit=result->basebit;
    const int t=result->t;
    const int b=result->b;

    CreateKeySwitchKey_fromArray(
        result->ks,
        out_key, out_key->params->alpha_min,
        in_key->key, n, t, basebit, b);
}


void BaseBExtra::KeySwitch(
    TLweSample * result,
    const BaseBKeySwitchKey * ks,
    const LweSample* sample)
{
    const TLweParams* params=ks->out_params;
    const int n=ks->n;
    const int basebit=ks->basebit;
    const int t=ks->t;
    const int mod = ks->b;
    int N_B = params->N/mod;

    tLweClear(result, params);
    for (int i = 0; i < N_B; i++)
    {
		result->b->coefsT[i] = sample->b;
	}

    BaseBExtra::KeySwitchTranslate_fromArray(
        result,
        (const TLweSample***) ks->ks, params,
        sample->a, n, t, basebit);
}

// Keyswitch adapted for base b

void BaseBExtra::KeySwitch_Id(
    TLweSample * result,
    const BaseBKeySwitchKey* ks,
    std::vector<LweSample*> samples)
{
    const TLweParams * params = ks->out_params;
    const int n = ks->n;
    const int basebit=ks->basebit;
    const int t=ks->t;
    const int N = params->N;
    const int mod = ks->b;
    int N_mod = N/mod;

    if (samples.size() > mod)
    {
      std::cout << "ERROR: Number of inputs for the keyswitch not right" << std::endl;
      return;
    }

    tLweClear(result, params);
    
    // the bs from each sample are replicated N_mod times in result
    
    for (int i = 0; i < samples.size(); i++)
    {
		for (int j = 0; j < N_mod; j++)
		{
			result->b->coefsT[i*N_mod + j] = samples[i]->b;
		}
	}

    int32_t ** as = (int32_t **) malloc(sizeof(int32_t*)*n);
    for (int i = 0; i < n; i++)
    {
		as[i] = (int32_t*) malloc(sizeof(int32_t)*samples.size());
	}
    for (int i = 0; i < n; i++)
    {
		for (int j = 0; j < samples.size(); j++)
		{
			as[i][j] = samples[j]->a[i];
		}
	}

    BaseBExtra::KeySwitchTranslate_fromArray_Generic(
        result,
        (const TLweSample ***) ks->ks,
        params,
        (const int32_t **) as, n, t, basebit, samples.size());

    // Clean up
    free(as);
}


void BaseBExtra::KeySwitchTranslate_fromArray_Generic(
    TLweSample * result,
    const TLweSample *** ks,
    const TLweParams * params,
    const int32_t ** as,
    const int n,
    const int t,
    const int basebit,
    const int M)
{
    const int base=1<<basebit;
    const int32_t prec_offset=int32_t(1)<<(32-(1+basebit*t)); //precision
    const int mask=base-1;

    for (int i = 0; i < n; i++)
    {
		for (int j = 0; j < M; j++)
		{
			const int32_t aijbar = as[i][j]+prec_offset;

			for (int p = 0; p < t; p++)
			{
				const int32_t aijp = (aijbar>>(32-(p+1)*basebit)) & mask;

				tLweSubMulTo(result, aijp, &ks[i][p][j], params);
			}
		}
    }
}
