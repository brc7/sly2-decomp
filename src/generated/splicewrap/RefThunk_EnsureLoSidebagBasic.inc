#include "splicewrap.h"
M2C_UNK EnsureLoSidebagBasic(s32, s32);                    /* extern */

CRef *RefThunk_EnsureLoSidebagBasic(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 0x15;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    EnsureLoSidebagBasic((s32)pbasic, arg0_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
