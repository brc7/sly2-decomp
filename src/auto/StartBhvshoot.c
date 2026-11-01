#include "m2c_prelude.h"
M2C_UNK SetBhvshootState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

void StartBhvshoot(s32 arg0) {
    StartBhv();
    SetBhvshootState(arg0, 0);
}
