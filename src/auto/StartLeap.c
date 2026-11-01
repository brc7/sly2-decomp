#include "m2c_prelude.h"
M2C_UNK LEAP_ctor(void *);                      /* extern */
M2C_UNK SetLeapState(void *, M2C_UNK);             /* extern */
M2C_UNK memset(void *, M2C_UNK, M2C_UNK);           /* extern */

void StartLeap(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    if (M2C_FIELD(arg0, s32 *, 4) != 0) {
        memset(arg3, 0, 0x70);
        memset(arg3, 0, 0x70);
        LEAP_ctor(arg3);
        M2C_FIELD(arg3, void **, 0x30) = arg0;
        M2C_FIELD(arg3, s32 *, 0x34) = arg1;
        M2C_FIELD(arg3, s32 *, 0x3C) = arg2;
        SetLeapState(arg3, 0);
    }
}
