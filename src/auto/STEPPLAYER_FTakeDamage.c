#include "m2c_prelude.h"
M2C_UNK FTakeJtDamage(s32);                         /* extern */

void STEPPLAYER_FTakeDamage(s32 arg0) {
    FTakeJtDamage(arg0 + 0xDF0);
}
