#include "m2c_prelude.h"
M2C_UNK SlotUnsettle(s32, s32);                    /* extern */

s32 func_001F88D8(void *arg0, s32 arg1) {
    if (arg1 == 1) {
        SlotUnsettle(M2C_FIELD(arg0, s32 *, 0x20), M2C_FIELD(arg0, s32 *, 0x24));
    }
    return M2C_FIELD(arg0, s32 *, 0x24);
}
