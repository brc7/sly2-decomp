#include "m2c_prelude.h"
f32 UMaxShape(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0xE8) * 4) + M2C_FIELD(arg0, s32 *, 0xEC)), f32 *, -4);
}
