#include "m2c_prelude.h"
M2C_UNK TriggerRumbleRumk__FP6RUMBLE4RUMKf(void *); /* extern */

void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void *arg0) {
    if ((M2C_FIELD(arg0, s32 *, 0x10E8) != 0) && (M2C_FIELD(arg0, s32 *, 0x1010) >= 3) && (M2C_FIELD(arg0, s32 *, 0x100C) == 3)) {
        TriggerRumbleRumk__FP6RUMBLE4RUMKf(arg0 + 0x10CC);
    }
}
