#include "splicewrap.h"
void ClearStepguardGeneratedTargets(s32);                         /* extern */
CRef *RefThunk_ClearStepguardGeneratedTargets(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    ClearStepguardGeneratedTargets((s32)pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
