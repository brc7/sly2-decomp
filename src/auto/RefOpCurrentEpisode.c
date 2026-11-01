#include "m2c_prelude.h"
s32 NEpisodeCur();                                /* extern */

void *RefOpCurrentEpisode(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = NEpisodeCur();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
