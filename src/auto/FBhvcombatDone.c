#include "m2c_prelude.h"
s32 FBhvcombatDone(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x34) == 0;
}
