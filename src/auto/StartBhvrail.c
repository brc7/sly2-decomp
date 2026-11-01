#include "m2c_prelude.h"
M2C_UNK SetBhvrailState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

void StartBhvrail(s32 arg0) {
    StartBhv();
    SetBhvrailState(arg0, 0);
}
