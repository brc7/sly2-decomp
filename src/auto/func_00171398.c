#include "m2c_prelude.h"
M2C_UNK func_00171258(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK ClearBhvBusy(void *);                      /* extern */
M2C_UNK func_00209970(s32, M2C_UNK);                /* extern */

void func_00171398(void *arg0) {
    CloseBhv();
    ClearBhvBusy(arg0);
    func_00171258(arg0, 0, 0, 0);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x21);
}
