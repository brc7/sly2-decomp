#include "m2c_prelude.h"
s32 GrfJoyPressed(void *arg0, s32 arg1) {
    return M2C_FIELD(arg0, s32 *, 0x10B8) & arg1;
}
