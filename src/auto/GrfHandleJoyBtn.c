#include "m2c_prelude.h"
s32 GrfJoyPressed();                                /* extern */
M2C_UNK SetJoyBtnHandled(s32, s32);                    /* extern */

s32 GrfHandleJoyBtn(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = GrfJoyPressed();
    SetJoyBtnHandled(arg0, arg1);
    return temp_s2;
}
