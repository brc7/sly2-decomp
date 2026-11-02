#include "splicewrap.h"
void func_001E1870(s32, s32, s32, s32, s32, s32, s32);          /* extern */
CRef *func_00109FB8(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[6] = { 6, 6, 4193, 0, 6, 0 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    s32 arg2_v;
    s32 arg3_v;
    s32 arg4_v;
    s32 arg5_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 6, aref, aotyp, &arg0_v, &arg1_v, &arg2_v, &arg3_v, &arg4_v, &arg5_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_001E1870(pbasic, arg0_v, arg1_v, arg2_v, arg3_v, arg4_v, arg5_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
