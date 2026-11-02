#include "splicewrap.h"


CRef *func_00100A00(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    void *temp_v0;
    aotyp[0] = 0x2002;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    temp_v0 = M2C_FIELD(pbasic, void **, 0xC);
    M2C_FIELD(temp_v0, void (**)(void *, s32), 0x134)(pbasic + M2C_FIELD(temp_v0, s16 *, 0x130), arg0_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
