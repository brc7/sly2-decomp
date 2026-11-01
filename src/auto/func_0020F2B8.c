#include "m2c_prelude.h"
M2C_UNK UpdateSwIllum();                            /* extern */

void func_0020F2B8(void *arg0, s128 arg1) {
    M2C_FIELD(arg0, s128 *, 0x3E10) = arg1;
    UpdateSwIllum();
}
