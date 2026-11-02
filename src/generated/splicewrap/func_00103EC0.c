#include "splicewrap.h"
void CRef_SetBool(void *, s32);                 /* extern */
s32 func_00171DA8(s32, s32);                        /* extern */

CRef *func_00103EC0(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 0x2071;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    CRef_SetBool(&ref, func_00171DA8(pbasic, arg0_v) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
