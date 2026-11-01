#include "m2c_prelude.h"
s32 IGadgetSlotFromId(s32, M2C_UNK);                    /* extern */
M2C_UNK SetStepplayerGadget(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_001DAF38(M2C_UNK, M2C_UNK);            /* extern */
extern s32 D_002E1E40;

void func_0014C258(void) {
    s32 temp_v0;

    if (D_002E1E40 != 0) {
        temp_v0 = IGadgetSlotFromId(D_002E1E40, 0x1F);
        if (temp_v0 != -1) {
            SetStepplayerGadget(D_002E1E40, temp_v0, -1);
        }
        func_001DAF38(0x1F, 0);
    }
}
