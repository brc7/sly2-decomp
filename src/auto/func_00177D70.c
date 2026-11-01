#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(void *, M2C_UNK);                 /* extern */

s32 func_00177D70(void *arg0) {
    s32 var_v0;

    var_v0 = 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(M2C_FIELD(arg0, void **, 0x10), 0x3C) != 0) {
        var_v0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x108C) != 0;
    }
    return var_v0;
}
