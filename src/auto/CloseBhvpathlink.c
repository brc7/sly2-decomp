#include "m2c_prelude.h"
M2C_UNK SetBhvpathlinkState(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

void CloseBhvpathlink(void *arg0) {
    CloseBhv();
    SetBhvpathlinkState(arg0, -1);
    ClearBhvBusy(arg0);
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x2C);
}
