#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(void *, M2C_UNK);                 /* extern */
M2C_UNK SubscribeLoObject();                            /* extern */

void func_00139900(void *arg0, void *arg1) {
    SubscribeLoObject();
    if (FIsBasicDerivedFrom__FP5BASIC3CID(arg1, 0x1D) != 0) {
        M2C_FIELD(arg0, void **, 0xE0) = arg1;
        M2C_FIELD(arg0, f32 *, 0x38) = (f32) M2C_FIELD(arg1, f32 *, 0x38);
    }
}
