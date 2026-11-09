#include "splicewrap.h"
void ResumeSnd(s32);                         /* extern */
CRef *RefThunk_ResumeSnd(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    ResumeSnd((s32)pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
