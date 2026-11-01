#include "m2c_prelude.h"
M2C_UNK func_00142860(void *);                      /* extern */
extern f32 D_002A4D60;

void SetStepplayerLeanTarget(void *arg0, s32 arg1, s32 arg2) {
    if ((arg2 != M2C_FIELD(arg0, s32 *, 0x10F8)) || (arg1 != M2C_FIELD(arg0, s32 *, 0x10FC))) {
        M2C_FIELD(arg0, s32 *, 0x10FC) = arg1;
        M2C_FIELD(arg0, s32 *, 0x10F8) = arg2;
        if (arg2 != 0) {
            if (M2C_FIELD(arg0, f32 *, 0x114C) != D_002A4D60) {
                M2C_FIELD(arg0, s32 *, 0x1148) = 1;
                func_00142860(arg0);
            }
        }
    }
}
