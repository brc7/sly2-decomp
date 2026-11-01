#include "m2c_prelude.h"
M2C_UNK PostLoadJtModeHud(s32);                         /* extern */

void PostLoadJtMode(s32 arg0) {
    PostLoadJtModeHud(arg0 + 0x24);
}
