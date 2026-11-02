#include "splicewrap.h"


CRef *func_0010FC80(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    void *temp_v0;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 0, aref, NULL);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    temp_v0 = M2C_FIELD(pbasic, void **, 0xC);
    M2C_FIELD(temp_v0, void (**)(void *), 0x29C)(pbasic + M2C_FIELD(temp_v0, s16 *, 0x298));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
