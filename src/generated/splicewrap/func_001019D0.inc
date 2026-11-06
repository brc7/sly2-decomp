#include "splicewrap.h"
void SetAsegAutoStart(s32, s32, f32, f32);          /* extern */
extern M2C_UNK D_002A3AD0;

CRef *func_001019D0(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[3] = { 1, 1, 4107 };
    CRef ref;
    f32 arg0_v;
    f32 arg1_v;
    s32 arg2_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 3, aref, aotyp, &arg0_v, &arg1_v, &arg2_v);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    SetAsegAutoStart(pbasic, arg2_v, arg0_v, arg1_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
