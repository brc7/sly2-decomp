#include "splicewrap.h"
void AddBreakableTaskStates(s32, s32, s32);               /* extern */
CRef *func_0010D0B0(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[2] = { 6, 4176 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 2, aref, aotyp, &arg0_v, &arg1_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    AddBreakableTaskStates((s32)pbasic, arg0_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
