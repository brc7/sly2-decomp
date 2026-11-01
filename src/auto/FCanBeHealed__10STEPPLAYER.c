#include "m2c_prelude.h"
s32 func_001A23C0(s32);                             /* extern */
s32 FCanPoBeHealed();                                /* extern */

s32 FCanBeHealed__10STEPPLAYER(s32 arg0) {
    if (FCanPoBeHealed() == 0) {
        return 0;
    }
    return func_001A23C0(arg0 + 0xDF0) != 2;
}
