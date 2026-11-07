#include "splicewrap.h"
void CRef_SetBool(void *, s32);                 /* extern */
s32 CanAcquireProxy(s32);                             /* extern */

CRef *RefThunk_CanAcquireProxy(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    CRef_SetBool(&ref, CanAcquireProxy(pbasic) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
