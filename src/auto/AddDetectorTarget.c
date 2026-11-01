#include "m2c_prelude.h"
M2C_UNK SetDetectorTargetState(void *, s32 *, M2C_UNK);      /* extern */
s32 PdteFindDetectorTarget();                                /* extern */

void AddDetectorTarget(void *arg0, s32 arg1) {
    s32 *temp_a1;
    u32 temp_v1;

    if (PdteFindDetectorTarget() == 0) {
        temp_v1 = M2C_FIELD(arg0, u32 *, 0xFC);
        if (temp_v1 < 8U) {
            M2C_FIELD(arg0, u32 *, 0xFC) = (u32) (temp_v1 + 1);
            temp_a1 = arg0 + ((temp_v1 * 0x10) + 0x100);
            *temp_a1 = arg1;
            SetDetectorTargetState(arg0, temp_a1, 0);
        }
    }
}
