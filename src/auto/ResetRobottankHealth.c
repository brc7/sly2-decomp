#include "m2c_prelude.h"
M2C_UNK SetRobottankState(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateRobottankHealthMeter();                            /* extern */
M2C_UNK SeekSma(void *, s32);                 /* extern */

void ResetRobottankHealth(void *arg0) {
    void *temp_a0;

    M2C_FIELD(arg0, s32 *, 0x560) = (s32) M2C_FIELD(arg0, s32 *, 0x55C);
    UpdateRobottankHealthMeter();
    temp_a0 = M2C_FIELD(arg0, void **, 0x570);
    if (temp_a0 != NULL) {
        SeekSma(temp_a0, *M2C_FIELD(M2C_FIELD(temp_a0, void **, 0x24), s32 **, 0x54));
    }
    SetRobottankState(arg0, 0);
}
