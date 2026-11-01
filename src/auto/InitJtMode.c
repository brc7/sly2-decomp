#include "m2c_prelude.h"
M2C_UNK InitJtModeHud(void *, void *);              /* extern */

void InitJtMode(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 0x20) = -1;
    InitJtModeHud(arg0 + 0x24, arg0);
}
