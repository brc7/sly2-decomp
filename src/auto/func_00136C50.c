#include "m2c_prelude.h"
M2C_UNK func_00141E70();                            /* extern */
s32 func_00141EA8(s32, M2C_UNK);                    /* extern */
s32 FIsPoCur(s32);                             /* extern */
M2C_UNK func_001F6978(s32, M2C_UNK);                /* extern */
M2C_UNK func_001F69F8(s32, M2C_UNK);                /* extern */

void func_00136C50(s32 arg0) {
    func_00141E70();
    if ((FIsPoCur(arg0) != 0) && (func_00141EA8(arg0, 0xC) != 0)) {
        func_001F6978(arg0, 0x24D);
        return;
    }
    func_001F69F8(arg0, 0x24D);
}
