#include "splicewrap.h"
void SetSndmgrChannelVolume(s32, s32, M2C_UNK, f32);      /* extern */
CRef *RefThunk_SetSndmgrChannelVolume(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 4178, 1 };
    CRef ref;
    s32 arg0_v;
    f32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    SetSndmgrChannelVolume(pbasic, arg0_v, 0, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
