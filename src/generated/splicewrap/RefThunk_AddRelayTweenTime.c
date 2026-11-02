#include "splicewrap.h"
void func_001E1510(s32, s32, f32);               /* extern */
CRef *RefThunk_AddRelayTweenTime(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 1, 0 };
    CRef ref;
    f32 arg0_v;
    s32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_001E1510(pbasic, arg1_v, arg0_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
