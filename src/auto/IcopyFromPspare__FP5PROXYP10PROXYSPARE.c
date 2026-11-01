#include "m2c_prelude.h"
s32 IcopyFromPspare__FP5PROXYP10PROXYSPARE(void *arg0, s32 arg1) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg0, void **, 0xE0);
    return ((s32) ((arg1 - M2C_FIELD(temp_a2, s32 *, 0x68)) * 0xCCCCCCCD) >> 4) + M2C_FIELD(temp_a2, s32 *, 0xC);
}
