#include "splicewrap.h"
void SetF32__4CReff(void *, f32);                 /* extern */
f32 func_001FE3F8(s32);                             /* extern */

CRef *func_00102538(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    SetF32__4CReff(&ref, func_001FE3F8(pbasic));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
