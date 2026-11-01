#include "m2c_prelude.h"
s32 CploExplgChildren(void *arg0, s32 *arg1) {
    *arg1 = M2C_FIELD(arg0, s32 *, 0xE4);
    return M2C_FIELD(arg0, s32 *, 0xE0);
}
