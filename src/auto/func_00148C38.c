#include "m2c_prelude.h"
M2C_UNK func_00148740(s32);                         /* extern */
M2C_UNK func_001490A0(s32, M2C_UNK);                /* extern */
M2C_UNK PostSoLoad();                            /* extern */

void func_00148C38(s32 arg0) {
    PostSoLoad();
    func_00148740(arg0 + 0x150);
    func_001490A0(arg0, 0);
}
