#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK func_0012A0B0(s32, M2C_UNK);                /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

s32 RefOpIsNullObj__FiP4CRefP6CFrame(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg2, s32 *, 0) == 0x21) {
        var_a1 = M2C_FIELD(arg2, s32 *, 4) == 0;
    }
    M2C_FIELD(arg0, s32 *, 4) = var_a1;
    M2C_FIELD(arg0, s32 *, 0) = 0x17;
    return arg0;
}
