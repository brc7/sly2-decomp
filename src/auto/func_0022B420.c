#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, M2C_UNK); /* extern */
M2C_UNK func_0022B090(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD490;

void func_0022B420(void *arg0) {
    s32 temp_v0;

    temp_v0 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, M2C_FIELD(arg0, s32 *, 0x58), 0);
    M2C_FIELD(arg0, s32 *, 0x54) = temp_v0;
    if ((temp_v0 == 0) || (M2C_FIELD(arg0, s32 *, 0) != 3)) {
        func_0022B090(arg0, 0);
        return;
    }
    func_0022B090(arg0, 2);
}
