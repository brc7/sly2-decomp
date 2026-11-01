#include "m2c_prelude.h"
M2C_UNK OnPoActive();                            /* extern */
M2C_UNK SetRobottankState(void *, M2C_UNK);             /* extern */

void OnRobottankActive(void *arg0, s32 arg1) {
    OnPoActive();
    if (arg1 != 0) {
        SetRobottankState(arg0, 2);
    } else if (M2C_FIELD(arg0, s32 *, 0x180) == 2) {
        SetRobottankState(arg0, 0);
    }
}
