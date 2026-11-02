#include "splicewrap.h"
void SetS32__4CRefi(void *, s32);                 /* extern */
s32 func_00154158(s32, s32);                        /* extern */

CRef *RefThunkDetector_GetDetectorTargetStateById(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 0x2002;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    SetS32__4CRefi(&ref, func_00154158(pbasic, arg0_v));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
