#include "m2c_prelude.h"
s32 FSwHandsOff(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x3D78) > 0;
}
