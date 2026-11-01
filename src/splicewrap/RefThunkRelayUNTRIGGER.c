#include "splicewrap.h"
void func_001DFEB8(s32);                         /* extern */
CRef *RefThunkRelayUNTRIGGER(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    func_001DFEB8(pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
