#include "m2c_prelude.h"
M2C_UNK SetBhvfollowState(s32, M2C_UNK);                /* extern */
M2C_UNK CloseBhv();                            /* extern */

void CloseBhvfollow(s32 arg0) {
    CloseBhv();
    SetBhvfollowState(arg0, -1);
}
