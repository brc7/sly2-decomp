#include "splicewrap.h"
void func_001B1B18(s32, s32, s32, s32);          /* extern */
extern M2C_UNK D_002A3F50;

CRef *func_0010C5F0(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    OTYP aotyp[3] = { 4150, 6, 4107 };
    CRef ref;
    s32 arg0_v;
    s32 arg1_v;
    s32 arg2_v;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 3, aref, aotyp, &arg0_v, &arg1_v, &arg2_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_001B1B18(pbasic, arg0_v, arg1_v, arg2_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
