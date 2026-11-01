#include "m2c_prelude.h"
void SetPoctlState(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x80) = arg1;
    if (arg1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x78) = 0xFFFF;
    }
}
