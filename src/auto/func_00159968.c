#include "m2c_prelude.h"
M2C_UNK SetLoParent_1A58D0(void *, s32, M2C_UNK);        /* extern */
s32 PsoFromLo(s32);                             /* extern */

void func_00159968(void *arg0) {
    SetLoParent_1A58D0(arg0, PsoFromLo(*M2C_FIELD(M2C_FIELD(arg0, void **, 0xE4), s32 **, 8)), 0);
}
