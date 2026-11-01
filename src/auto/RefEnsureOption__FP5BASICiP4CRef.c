#include "m2c_prelude.h"
s32 FFindBinding__8CSidebagiP4CRef(s32, s32, M2C_UNK); /* extern */
M2C_UNK RefSetBinding__8CSidebagiP4CRef(s32, s32, s32, s32); /* extern */
M2C_UNK CSidebag_RefAddBinding(s32, s32, s32, s32);          /* extern */
M2C_UNK EnsureBasicSidebag__FP5BASIC(void *);                      /* extern */

s32 RefEnsureOption__FP5BASICiP4CRef(s32 arg0, void *arg1, s32 arg2, s32 arg3) {
    EnsureBasicSidebag__FP5BASIC(arg1);
    if (FFindBinding__8CSidebagiP4CRef(M2C_FIELD(arg1, s32 *, 4), arg2, 0) != 0) {
        RefSetBinding__8CSidebagiP4CRef(arg0, M2C_FIELD(arg1, s32 *, 4), arg2, arg3);
    } else {
        CSidebag_RefAddBinding(arg0, M2C_FIELD(arg1, s32 *, 4), arg2, arg3);
    }
    return arg0;
}
