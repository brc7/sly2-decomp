#include "m2c_prelude.h"
M2C_UNK CSidebag_RefAddBinding(s32, s32, s32, s32);          /* extern */
M2C_UNK EnsureBasicSidebag__FP5BASIC(void *);                      /* extern */

s32 RefAddOption__FP5BASICiP4CRef(s32 arg0, void *arg1, s32 arg2, s32 arg3) {
    EnsureBasicSidebag__FP5BASIC(arg1);
    CSidebag_RefAddBinding(arg0, M2C_FIELD(arg1, s32 *, 4), arg2, arg3);
    return arg0;
}
