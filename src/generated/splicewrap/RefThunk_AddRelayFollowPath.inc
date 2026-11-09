#include "splicewrap.h"
void AddRelayFollowPath(s32, s32, s32, s32, f32, s32, s32);     /* extern */
CRef *RefThunk_AddRelayFollowPath(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[6] = { 6, 0, 0, 1, 6, 6 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    s32 arg2_v;
    f32 arg3_v;
    s32 arg4_v;
    s32 arg5_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 6, aref, aotyp, &arg0_v, &arg1_v, &arg2_v, &arg3_v, &arg4_v, &arg5_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    AddRelayFollowPath((s32)pbasic, arg0_v, arg1_v, arg2_v, arg3_v, arg4_v, arg5_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
