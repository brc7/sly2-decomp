#include "m2c_prelude.h"
s32 SO_PmatWorld(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x60) + 0x40;
}
