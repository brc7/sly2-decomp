#include "splicewrap.h"
void SetDetectorTargetStateById(s32, s32, s32);               /* extern */
CRef *RefThunkStepguard_SetDetectorTargetStateById(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 8194, 4203 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    SetDetectorTargetStateById(M2C_FIELD(pbasic, s32 *, 0xF64), arg0_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
