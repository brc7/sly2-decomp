#include "m2c_prelude.h"
M2C_UNK PloAcquireProxy(s32, M2C_UNK);                /* extern */
s32 func_001CB2E0();                                /* extern */

void EnsureProxyCopy(s32 arg0) {
    if (func_001CB2E0() == 0) {
        PloAcquireProxy(arg0, 1);
    }
}
