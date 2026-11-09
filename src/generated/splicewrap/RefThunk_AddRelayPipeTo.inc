#include "splicewrap.h"
void AddRelayPipeTo(s32, s32, s32, s32, f32);     /* extern */
CRef *RefThunk_AddRelayPipeTo(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[4] = { 6, 1, 0, 6 };
    CRef ref;
    s32 arg0_v;
    f32 arg1_v;
    s32 arg2_v;
    s32 arg3_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 4, aref, aotyp, &arg0_v, &arg1_v, &arg2_v, &arg3_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    AddRelayPipeTo((s32)pbasic, arg0_v, arg2_v, arg3_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
