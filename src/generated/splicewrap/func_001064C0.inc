#include "splicewrap.h"
void AddGuardScriptBusyOff(s32);                         /* extern */
CRef *func_001064C0(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    AddGuardScriptBusyOff(pbasic);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
