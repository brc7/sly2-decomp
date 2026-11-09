#include "splicewrap.h"
void CRef_SetBool(void *, s32);                 /* extern */
s32 CanGrabspotAcceptPlayer(s32, s32);                        /* extern */

CRef *func_0010D660(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 0x2002;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    CRef_SetBool(&ref, CanGrabspotAcceptPlayer((s32)pbasic, arg0_v) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
