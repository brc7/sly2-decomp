#include "splicewrap.h"
void func_00155530(s32, s32, s32, s32, s32);          /* extern */
CRef *func_0010F6E8(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[4] = { 6, 6, 6, 6 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    s32 arg2_v;
    s32 arg3_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 4, aref, aotyp, &arg0_v, &arg1_v, &arg2_v, &arg3_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_00155530(pbasic, arg0_v, arg1_v, arg2_v, arg3_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
