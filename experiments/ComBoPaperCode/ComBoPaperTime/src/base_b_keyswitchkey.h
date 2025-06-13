#ifndef _BASE_B_KEYSWITCHKEY_H
#define _BASE_B_KEYSWITCHKEY_H

#include "tfhe.h"


struct BaseBKeySwitchKey {
    int32_t n;            ///< length of the input key: s'
    int32_t t;            ///< decomposition length
    int32_t basebit;      ///< log_2(base)
    int32_t b;            /// base used for digits
    int32_t base;         ///< decomposition base: a power of 2
    const TLweParams * out_params; ///< params of the output key s
    TLweSample * ks0_raw;  //tableau qui contient tout les TLwe samples de taille n.t.b
    TLweSample *** ks; ///< the keyswitch elements: a n.t.b matrix
    // de taille n pointe vers ks0 un tableau dont les cases sont espaceés de t positions

    BaseBKeySwitchKey(int32_t n, int32_t t, int32_t basebit, int32_t b, const TLweParams* out_params, TLweSample* ks0_raw);
    ~BaseBKeySwitchKey();
    BaseBKeySwitchKey(const BaseBKeySwitchKey&) = delete;
    void operator=(const BaseBKeySwitchKey&) = delete;

};


BaseBKeySwitchKey* alloc_BaseBKeySwitchKey();

void free_BaseBKeySwitchKey(BaseBKeySwitchKey* ptr);

void init_BaseBKeySwitchKey(BaseBKeySwitchKey* obj, int n, int t, int basebit, int b, const TLweParams* out_params);

void destroy_BaseBKeySwitchKey(BaseBKeySwitchKey* obj);

BaseBKeySwitchKey* new_BaseBKeySwitchKey(int n, int t, int basebit, int b, const TLweParams* out_params);

void delete_BaseBKeySwitchKey(BaseBKeySwitchKey* obj);


#endif //_BASE_B_KEYSWITCHKEY_H
