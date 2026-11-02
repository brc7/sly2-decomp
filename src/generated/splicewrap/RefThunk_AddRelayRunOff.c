#include "splicewrap.h"
void AddRelayRunOff(s32);                         /* extern */
CRef *RefThunk_AddRelayRunOff(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    AddRelayRunOff(pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
