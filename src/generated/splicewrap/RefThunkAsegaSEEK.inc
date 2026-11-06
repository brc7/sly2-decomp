#include "splicewrap.h"
void func_00130D30(s32, s32, f32, f32);          /* extern */
extern M2C_UNK D_002A3AF8;

CRef *RefThunkAsegaSEEK(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[3] = { 4108, 1, 1 };
    CRef ref;
    s32 arg0_v;
    f32 arg1_v;
    f32 arg2_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 3, aref, aotyp, &arg0_v, &arg1_v, &arg2_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    func_00130D30(pbasic, arg0_v, arg1_v, arg2_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
