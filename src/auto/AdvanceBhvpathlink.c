#include "m2c_prelude.h"
M2C_UNK SetBhvpathlinkState(void *, M2C_UNK);             /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */

void AdvanceBhvpathlink(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x34) + 1;
    M2C_FIELD(arg0, s32 *, 0x34) = temp_v0;
    if (temp_v0 < M2C_FIELD(M2C_FIELD(arg0, void **, 0x30), s32 *, 0x50)) {
        SetBhvpathlinkState(arg0, -1);
        SetBhvpathlinkState(arg0, 0);
        return;
    }
    SetBhvpathlinkState(arg0, -1);
    ClearBhvBusy(arg0);
}
