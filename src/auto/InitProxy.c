#include "m2c_prelude.h"
M2C_UNK SetLoBusyk(s32, M2C_UNK);                /* extern */
M2C_UNK InitXfm();                            /* extern */

void InitProxy(s32 arg0) {
    InitXfm();
    SetLoBusyk(arg0, 0);
}
