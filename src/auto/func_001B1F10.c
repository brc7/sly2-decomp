#include "m2c_prelude.h"
M2C_UNK ReleaseAct(s32);                         /* extern */
M2C_UNK ApplyBombThrow();                            /* extern */

void func_001B1F10(s32 arg0) {
    ApplyBombThrow();
    ReleaseAct(arg0 + 0x268);
}
