#include "m2c_prelude.h"
M2C_UNK SetBhvfollowState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

void StartBhvfollow(s32 arg0) {
    StartBhv();
    SetBhvfollowState(arg0, 0);
}
