#include "splicewrap.h"
void EnsureLoSidebagInt__FP2LO5OPTIDi(s32, s32, s32);               /* extern */
CRef *RefThunkBasicCUSTOM_INT_PROPERTY(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 21, 5 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    EnsureLoSidebagInt__FP2LO5OPTIDi(pbasic, arg0_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
