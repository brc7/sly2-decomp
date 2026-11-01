#include "m2c_prelude.h"
M2C_UNK SetBhvshootState(s32, M2C_UNK);                /* extern */
M2C_UNK CloseBhv();                            /* extern */

void CloseBhvshoot(s32 arg0) {
    CloseBhv();
    SetBhvshootState(arg0, -1);
}
