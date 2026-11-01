#include "m2c_prelude.h"
s32 POCTL_FButtonDown(void *arg0, s32 arg1) {
    return (M2C_FIELD(arg0, s32 *, 0x30) & arg1) != 0;
}
