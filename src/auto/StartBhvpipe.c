#include "m2c_prelude.h"
M2C_UNK SetBhvpipeState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

void StartBhvpipe(s32 arg0) {
    StartBhv();
    SetBhvpipeState(arg0, 0);
}
