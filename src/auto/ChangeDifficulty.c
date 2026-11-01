#include "m2c_prelude.h"
f32 GGetDifficulty();                                /* extern */
M2C_UNK SetDifficulty(s32, f32);                    /* extern */

void ChangeDifficulty(s32 arg0, f32 fparg0) {
    SetDifficulty(arg0, GGetDifficulty() + fparg0);
}
