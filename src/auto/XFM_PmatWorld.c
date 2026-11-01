#include "m2c_prelude.h"
M2C_UNK UpdateXfmMatWorld();                            /* extern */

s32 XFM_PmatWorld(s32 arg0) {
    UpdateXfmMatWorld();
    return arg0 + 0xA0;
}
