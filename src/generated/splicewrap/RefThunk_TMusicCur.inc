#include "splicewrap.h"
void SetF32__4CReff(void *, f32);                 /* extern */
f32 TMusicCur(s32);                             /* extern */

CRef *RefThunk_TMusicCur(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    SetF32__4CReff(&ref, TMusicCur(pbasic));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
