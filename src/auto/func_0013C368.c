#include "m2c_prelude.h"
M2C_UNK SetBootyState(void *, M2C_UNK);             /* extern */
s32 func_001F7588();                                /* extern */

s32 func_0013C368(void *arg0) {
    if ((func_001F7588() == 0) || (M2C_FIELD(arg0, s32 *, 0x184) != 5)) {
        return 0;
    }
    SetBootyState(arg0, 0);
    return 1;
}
