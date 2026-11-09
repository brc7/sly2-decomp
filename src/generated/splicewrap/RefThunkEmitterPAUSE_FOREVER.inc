#include "splicewrap.h"
void PauseEmitterIndefinite__FP7EMITTER(s32);    /* extern */
CRef *RefThunkEmitterPAUSE_FOREVER(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    PauseEmitterIndefinite__FP7EMITTER((s32)pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
