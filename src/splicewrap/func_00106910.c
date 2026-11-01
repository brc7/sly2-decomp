#include "splicewrap.h"
s32 FWipingAseg__FP4ASEG(s32, s32);                 /* extern */
void CRef_SetBool(void *, s32);                 /* extern */
CRef *func_00106910(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 6;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    CRef_SetBool(&ref, FWipingAseg__FP4ASEG(pbasic, arg0_v) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
