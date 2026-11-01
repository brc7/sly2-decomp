#include "splicewrap.h"
void CRef_SetBool(void *, s32);                 /* extern */
CRef *RefThunk_FInvulnerablePo(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    void *temp_v1;
    aotyp[0] = 0x1011;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    temp_v1 = M2C_FIELD(pbasic, void **, 0xC);
    CRef_SetBool(&ref, M2C_FIELD(temp_v1, s32 (**)(void *, s32), 0x264)(pbasic + M2C_FIELD(temp_v1, s16 *, 0x260), arg0_v) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
