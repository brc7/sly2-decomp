#include "m2c_prelude.h"
void SetStepguardHpLevel(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x33C) = (s32) ((arg1 + 1) * 0xA);
}
