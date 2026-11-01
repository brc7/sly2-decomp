#include "m2c_prelude.h"
M2C_UNK memcpy(s32, s32, s32);                      /* extern */

s32 CbRead__17CQueueInputMemoryiPv(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;
    s32 temp_s0_2;

    temp_s0_2 = M2C_FIELD(arg0, s32 *, 0x10);
    temp_s0 = (arg1 < temp_s0_2) ? arg1 : temp_s0_2;
    memcpy(arg2, M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC), temp_s0);
    M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + temp_s0);
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) - temp_s0);
    return temp_s0;
}
