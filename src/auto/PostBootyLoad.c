#include "m2c_prelude.h"
M2C_UNK SetBootyState(s32, M2C_UNK);                /* extern */
M2C_UNK PostSoLoad();                            /* extern */

void PostBootyLoad(s32 arg0) {
    PostSoLoad();
    SetBootyState(arg0, 0);
}
