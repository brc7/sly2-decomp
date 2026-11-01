#include "m2c_prelude.h"
M2C_UNK ClonePo();                            /* extern */
M2C_UNK CloneBhvstk(s32, s32);                    /* extern */

void CloneStep(s32 arg0) {
    ClonePo();
    CloneBhvstk(arg0 + 0x26C, arg0);
}
