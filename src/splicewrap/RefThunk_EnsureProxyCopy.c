#include "splicewrap.h"
void CRef_SetBasic(void *, s32);                 /* extern */
s32 func_001CB328(s32);                             /* extern */

CRef *RefThunk_EnsureProxyCopy(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    CRef_SetBasic(&ref, func_001CB328(pbasic));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
