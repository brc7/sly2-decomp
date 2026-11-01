#include "m2c_prelude.h"
s32 func_0012D058();                                /* extern */
M2C_UNK AddXfAct(s32, s32, void *);            /* extern */

void func_0012CFE0(void *arg0) {
    if (func_0012D058() == 0) {
        AddXfAct(M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8), arg0);
    }
}
