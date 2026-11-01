#include "m2c_prelude.h"
M2C_UNK func_001CB518();                            /* extern */
M2C_UNK SetRcPickupState(s32, M2C_UNK);                /* extern */

void func_001D4F10(s32 arg0) {
    func_001CB518();
    SetRcPickupState(arg0, -1);
    SetRcPickupState(arg0, 0);
}
