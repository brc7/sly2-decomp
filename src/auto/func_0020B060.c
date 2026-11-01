#include "m2c_prelude.h"
M2C_UNK ResolveAnimBinding(void *, s32);                 /* extern */
M2C_UNK func_0020AFF0();                            /* extern */

void func_0020B060(void *arg0) {
    func_0020AFF0();
    ResolveAnimBinding(arg0 + 0xB0, M2C_FIELD(arg0, s32 *, 0xAC));
}
