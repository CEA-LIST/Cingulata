#include "base_b_keyswitchkey.h"


BaseBKeySwitchKey::BaseBKeySwitchKey(
        const int n,
        const int t,
        const int basebit,
        const int b,
        const TLweParams* out_params,
        TLweSample* ks0_raw)
        :
        n(n),
        t(t),
        b(b),
        basebit(basebit),
        base(1<<basebit),
        out_params(out_params),
        ks0_raw(ks0_raw)
{
    ks = new TLweSample**[n];
    for (int i = 0; i < n; ++i)
    {
		ks[i] = new TLweSample*[t];
		for (int j = 0; j < t; j++)
		{
			ks[i][j] = ks0_raw + i*t*b + j*b;
		}
	}
}


BaseBKeySwitchKey::~BaseBKeySwitchKey() {
    delete[] ks;
}

/**
 * BaseBKeySwitchKey constructor function
 */

void init_BaseBKeySwitchKey(BaseBKeySwitchKey* obj, int n, int t, int basebit, int b, const TLweParams* out_params) {
    TLweSample* ks0_raw = new_TLweSample_array(n*t*b, out_params);

    new(obj) BaseBKeySwitchKey(n,t,basebit,b,out_params, ks0_raw);
}

/**
 * BaseBKeySwitchKey destructor
 */
void destroy_BaseBKeySwitchKey(BaseBKeySwitchKey* obj) {
    const int n = obj->n;
    const int t = obj->t;
    const int b = obj->b;
    delete_TLweSample_array(n*t*b,obj->ks0_raw);

    obj->~BaseBKeySwitchKey();
}



BaseBKeySwitchKey* alloc_BaseBKeySwitchKey() {
    return (BaseBKeySwitchKey*) malloc(sizeof(BaseBKeySwitchKey));
}

void free_BaseBKeySwitchKey(BaseBKeySwitchKey* ptr) {
    free(ptr);
}
 
BaseBKeySwitchKey* new_BaseBKeySwitchKey(int n, int t, int basebit, int b, const TLweParams* out_params) {
    BaseBKeySwitchKey* obj = alloc_BaseBKeySwitchKey();
    init_BaseBKeySwitchKey(obj, n, t, basebit, b, out_params);
    return obj;
}

void delete_BaseBKeySwitchKey(BaseBKeySwitchKey* obj) {
    destroy_BaseBKeySwitchKey(obj);
    free_BaseBKeySwitchKey(obj);
}
