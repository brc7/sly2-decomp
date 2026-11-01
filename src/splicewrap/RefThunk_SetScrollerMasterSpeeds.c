#include "splicewrap.h"
void SetScrollerMasterSpeeds__FP8SCROLLERff(s32, f32, f32); /* extern */
CRef *RefThunk_SetScrollerMasterSpeeds(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 1, 1 };
    CRef ref;
    f32 arg0_v;
    f32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    SetScrollerMasterSpeeds__FP8SCROLLERff(pbasic, arg0_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
