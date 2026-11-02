#include "splicewrap.h"
void PauseEmitter__FP7EMITTERf(s32, f32);        /* extern */
CRef *RefThunkEmitterPAUSE(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    f32 arg0_v;
    aotyp[0] = 1;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    PauseEmitter__FP7EMITTERf(pbasic, arg0_v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
