#include "m2c_prelude.h"
M2C_UNK SetBhvattackState(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetBhvBusy(s32);                         /* extern */

void StartBhvattack(s32 arg0) {
    StartBhv();
    SetBhvattackState(arg0, 0);
    SetBhvBusy(arg0);
}
