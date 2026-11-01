#include "m2c_prelude.h"
s32 GrfPressedWithin__5POCTLUif();                                /* extern */
M2C_UNK ClearPoctlJoyPressed(s32, s32);                    /* extern */

s32 GrfConsumePressed__5POCTLUif(s32 arg0) {
    s32 temp_v0;

    temp_v0 = GrfPressedWithin__5POCTLUif();
    ClearPoctlJoyPressed(arg0, temp_v0);
    return temp_v0;
}
