#ifndef _BASE_B_KEYSWITCH_H
#define _BASE_B_KEYSWITCH_H

// keyswitch lwe -> trlwe avec répétitions

#include "base_b_keyswitchkey.h"

struct BaseBExtra
{
public:
    static void CreateKeySwitchKey_fromArray(
        TLweSample*** result,
        const TLweKey* out_key,
        const double out_alpha,
        const int* in_key,
        const int n,
        const int t,
        const int basebit,
        const int mod);

    static void KeySwitchTranslate_fromArray(
        TLweSample* result,
        const TLweSample*** ks,
        const TLweParams* params,
        const Torus32* ai,
        const int n,
        const int t,
        const int basebit);

    static void KeySwitchTranslate_fromArray_Generic(
        TLweSample * result,
        const TLweSample *** ks,
        const TLweParams * params,
        const Torus32 ** as,
        const int n,
        const int t,
        const int basebit,
        const int M);

    /**
     * creates a Key Switching Key between the two keys
     */
    static void CreateKeySwitchKey(
        BaseBKeySwitchKey* result,
        const LweKey* in_key,
        const TLweKey* out_key);

    /**
     * applies keySwitching
     */
    static void KeySwitch(
        TLweSample* result,
        const BaseBKeySwitchKey* ks,
        const LweSample* sample);

    static void KeySwitch_Id(
        TLweSample * result,
        const BaseBKeySwitchKey* ks,
        std::vector<LweSample*> samples);
};

#endif //_BASE_B_KEYSWITCH_H
