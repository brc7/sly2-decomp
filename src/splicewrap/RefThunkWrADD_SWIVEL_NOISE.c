#include "splicewrap.h"
void AddWrSwivelNoise__FP2WRffff(s32, f32, f32, f32, f32); /* extern */
CRef *RefThunkWrADD_SWIVEL_NOISE(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[4] = { 1, 1, 1, 1 };
    CRef ref;
    f32 arg0_v;
    f32 arg1_v;
    f32 arg2_v;
    f32 arg3_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 4, aref, aotyp, &arg0_v, &arg1_v, &arg2_v, &arg3_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    AddWrSwivelNoise__FP2WRffff(pbasic, arg0_v, arg1_v, arg2_v, arg3_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
