#include "splicewrap.h"
void func_00170C20(s32, f32);                    /* extern */
CRef *func_00106010(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    f32 arg0_v;
    aotyp[0] = 1;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_00170C20(pbasic, arg0_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
