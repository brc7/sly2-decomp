#include "m2c_prelude.h"
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern M2C_UNK D_004520A8;

s32 RefOpBitAnd(void *arg0, s32 arg1, void *arg2) {
    s32 temp_v0;
    s32 var_a1;
    void *var_v1;

    var_a1 = -1;
    var_v1 = NULL;
    if (M2C_FIELD(arg2, s32 *, 0) != 0) {
        var_v1 = M2C_FIELD(arg2, void **, 4);
    }
    if (var_v1 != NULL) {
        do {
            temp_v0 = M2C_FIELD(var_v1, s32 *, 0xC);
            var_v1 = M2C_FIELD(var_v1, void **, 0x10);
            var_a1 &= temp_v0;
        } while (var_v1 != NULL);
    }
    M2C_FIELD(arg0, s32 *, 4) = var_a1;
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return (s32) arg0;
}
