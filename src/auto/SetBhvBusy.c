#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
M2C_UNK func_0020B438(s32);                         /* extern */

void SetBhvBusy(void *arg0) {
    void *var_a0;

    if (M2C_FIELD(arg0, s32 *, 0x1C) == 0) {
        M2C_FIELD(arg0, s32 *, 0x1C) = 1;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(M2C_FIELD(arg0, s32 *, 0x10), 0x38) != 0) {
            func_0020B438(M2C_FIELD(arg0, s32 *, 0x10));
        }
        var_a0 = M2C_FIELD(arg0, void **, 0x14);
        if (var_a0 != NULL) {
            do {
                M2C_FIELD(var_a0, s32 *, 0x20) = (s32) (M2C_FIELD(var_a0, s32 *, 0x20) + 1);
                var_a0 = M2C_FIELD(var_a0, void **, 0x14);
            } while (var_a0 != NULL);
        }
    }
}
