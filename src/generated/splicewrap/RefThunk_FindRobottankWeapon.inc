#include "splicewrap.h"
void CRef_SetBasic(void *, s32);                 /* extern */
s32 FindRobottankWeapon(s32, s32);                        /* extern */

CRef *RefThunk_FindRobottankWeapon(CRef *ret, void *pbasic, s32 cref, CRef *aref)
{
    CRef ref;
    OTYP aotyp[1];
    s32 arg0_v;
    aotyp[0] = 6;
    RefSetArgsFromSplice__FiP4CRefP4OTYPe(&ref, 1, aref, aotyp, &arg0_v);
    CRef_SetBasic(&ref, FindRobottankWeapon((s32)pbasic, arg0_v));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
