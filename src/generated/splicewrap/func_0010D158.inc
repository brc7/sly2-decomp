#include "splicewrap.h"
void ScheduleBreakableReaction(s32, M2C_UNK, M2C_UNK);       /* extern */
CRef *func_0010D158(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    ScheduleBreakableReaction((s32)pbasic, 0, 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
