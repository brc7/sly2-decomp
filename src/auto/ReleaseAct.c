#include "m2c_prelude.h"
M2C_UNK func_0012D020(s32);                         /* extern */
M2C_UNK func_0012E210(s32);                         /* extern */

void ReleaseAct(s32 *arg0) {
    s32 temp_s0;

    temp_s0 = *arg0;
    if (temp_s0 != 0) {
        func_0012D020(temp_s0);
        func_0012E210(temp_s0);
        *arg0 = 0;
    }
}
