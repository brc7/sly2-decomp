#include "m2c_prelude.h"
void SetJoyBtnHandled(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x10B8) = (s32) (M2C_FIELD(arg0, s32 *, 0x10B8) & ~arg1);
    M2C_FIELD(arg0, s32 *, 0x10BC) = (s32) (M2C_FIELD(arg0, s32 *, 0x10BC) | (M2C_FIELD(arg0, s32 *, 0x10B4) & arg1));
}
