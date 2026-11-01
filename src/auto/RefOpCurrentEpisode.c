#include "m2c_prelude.h"
s32 func_00210A98();                                /* extern */

void *RefOpCurrentEpisode(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210A98();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
